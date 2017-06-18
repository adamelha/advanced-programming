#include "thread_manager.h"


// Thread that should take one game from the list, instantiate the algorithms and and manage the game
void ThreadManager::threadGameFunc()
{
	BattleScore battleScore;
	int indexOfGame, playerARound, playerBRound;
	int playerAIndex, playerBIndex;

	while( true )  // as long there are gamesto be played, play
	{ 
		indexOfGame = atomicCounter.fetch_add(1, std::memory_order_relaxed);
		if (indexOfGame >= listOfGames.size())
			break;

		playerAIndex = listOfGames[indexOfGame].first;
		playerBIndex = listOfGames[indexOfGame].second;
		IBattleshipGameAlgo * firstPlayer  =  ( (GetAlgoFuncType)GetProcAddress(dllList[playerAIndex], "GetAlgorithm") )  () ;
		IBattleshipGameAlgo * secondPlayer =   ((GetAlgoFuncType)GetProcAddress(dllList[playerBIndex], "GetAlgorithm") ) () ;
		
		DEBUG_PRINT("Pointer to firstPlayer: %p\n", firstPlayer);
		DEBUG_PRINT("Pointer to secondPlayer: %p'n", secondPlayer);

		// Play game get score
		Battle battle(player_A);
		battleScore = battle.War(board, firstPlayer, secondPlayer);
		
		// Increment player rounds
		playerARound = playerRound[playerAIndex].fetch_add(1, std::memory_order_relaxed);
		playerBRound = playerRound[playerBIndex].fetch_add(1, std::memory_order_relaxed);

		// Update score for player A and B for this round
		// Wait for prev round to update its score
		while (scoreTabel[playerARound - 1][playerAIndex].totalPointsAgainst == -1) {}
		while (scoreTabel[playerBRound - 1][playerBIndex].totalPointsAgainst == -1) {}

		if (battleScore.winner == Winner::PlayerA)
		{
			scoreTabel[playerARound][playerAIndex].wins = scoreTabel[playerARound - 1][playerAIndex].wins + 1;
			scoreTabel[playerBRound][playerBIndex].losses = scoreTabel[playerBRound - 1][playerBIndex].losses + 1;
		}
		else if (battleScore.winner == Winner::PlayerB)
		{
			scoreTabel[playerBRound][playerBIndex].wins = scoreTabel[playerBRound - 1][playerBIndex].wins + 1;
			scoreTabel[playerBRound][playerBIndex].losses = scoreTabel[playerBRound - 1][playerBIndex].losses;
			
			scoreTabel[playerARound][playerAIndex].losses = scoreTabel[playerARound - 1][playerAIndex].losses + 1;
			scoreTabel[playerARound][playerAIndex].wins = scoreTabel[playerARound - 1][playerAIndex].wins;

		}
		
		else 
		{
			INFO_PRINT("TIE\n");
			scoreTabel[playerBRound][playerBIndex].wins = scoreTabel[playerBRound - 1][playerBIndex].wins;
			scoreTabel[playerBRound][playerBIndex].losses = scoreTabel[playerBRound - 1][playerBIndex].losses;

			scoreTabel[playerARound][playerAIndex].losses = scoreTabel[playerARound - 1][playerAIndex].losses;
			scoreTabel[playerARound][playerAIndex].wins = scoreTabel[playerARound - 1][playerAIndex].wins;

			return;
		}

		scoreTabel[playerARound][playerAIndex].totalPointsFor = scoreTabel[playerARound - 1][playerAIndex].totalPointsFor + battleScore.playerAPoints;
		scoreTabel[playerBRound][playerBIndex].totalPointsFor = scoreTabel[playerBRound - 1][playerBIndex].totalPointsFor + battleScore.playerBPoints;

		scoreTabel[playerARound][playerAIndex].totalPointsAgainst = scoreTabel[playerARound - 1][playerAIndex].totalPointsAgainst + battleScore.playerBPoints;
		scoreTabel[playerBRound][playerBIndex].totalPointsAgainst = scoreTabel[playerBRound - 1][playerBIndex].totalPointsAgainst + battleScore.playerAPoints;

	}
}


ThreadManager::ThreadManager(const string& _path, const Board& _board, int _numberOfThreads) : path(_path), board(_board), numberOfThreads(_numberOfThreads)
{
	threadList.resize(numberOfThreads); 
	loadDllFiles();
	creatListOfGames();
	initial_numberOfRounds_numberOfPlayers_scoreTabel_playerRound();
	// just extract all of this code to initial_numberOfRounds_numberOfPlayers_scoreTabel_playerRound function so it will look better

	/*numberOfRounds = (dllList.size() - 1) * 2;
	numberOfPlayers = dllList.size();

	// init score tabel
	
	// size numberOfRounds + 1 because we allocate a round 0 where the scores are 0 (simplifies algorithm).
	scoreTabel.resize(numberOfRounds + 1);
	//scoreTabel = new std::vector<PlayerScore>[numberOfRounds + 1];
	//scoreTabel = new PlayerScore*[numberOfRounds + 1];
	for (size_t i = 0; i < numberOfRounds + 1; i++)
	{
		// Each vector is of the size of the amount of players
		scoreTabel[i].resize(numberOfPlayers);
		//scoreTabel[i] = new PlayerScore[numberOfPlayers];
		for (size_t j = 0; j < numberOfPlayers; j++)
		{
			if (i == 0) {
				scoreTabel[i][j].totalPointsFor = 0;
				scoreTabel[i][j].totalPointsAgainst = 0;
			}
			else {
				scoreTabel[i][j].totalPointsFor = -1;
				scoreTabel[i][j].totalPointsAgainst = -1;
			}
			
			scoreTabel[i][j].wins = 0;
			scoreTabel[i][j].losses = 0;
		}
	}

	playerRound = new std::atomic<int>[numberOfPlayers];
	for (size_t i = 0; i < numberOfPlayers; i++)
	{
		playerRound[i] = 1;
	}
	// Init rounds
	*/

}

// This methods creates and runs the threads that call Battle::war() which will create instances of the 2 algos it's supposed to use
status_t ThreadManager::run()
{
	// Create threads to run game
	for (size_t i = 0; i < numberOfThreads; i++)
	{
		threadList[i] = std::thread(&ThreadManager::threadGameFunc, this);
	}

	int curRound = 1;
	// Here main thread should do his work printing scores
	
	while (1) 
	{
		// Scan current round until all players are updated
		for (size_t i = 1; i < numberOfPlayers; i++)
		{
			if (scoreTabel[curRound][i].totalPointsAgainst == -1)
			{
				// found unupdated entry, start over
				break;
			}

			// All players in round updated
			if (i == numberOfPlayers - 1)
			{
				
				// Round completed, print results
				INFO_PRINT("displaying tabel for round %d:\n", curRound);
				display.displayTabel(scoreTabel[curRound]);
				curRound++;
			}
		}


		if (curRound == numberOfRounds + 1)
		{
			break;
		}
	}

	DEBUG_PRINT("Finished all rounds!!!\n");

	// Make sure all threads finished
	for (size_t i = 0; i < numberOfThreads; i++)
	{
		threadList[i].join();
	}
	
	return STATUS_OK;
}

/* Private Methods*/

bool ThreadManager::loadDllFiles()
{
	HANDLE dir;
	WIN32_FIND_DATAA fileData; //data struct for file
	string fileName, fullFileName;
	int playerNumber = 0;
	bool init_failed;
	bool status = true;
	// define function of the type we expect
	//typedef IBattleshipGameAlgo *(*GetAlgoFuncType)();
	//GetAlgoFuncType getAlgoFunc;

	ErrorClass errmsgs(NUM_OF_DLL_ERR_MSGS);

	// iterate over *.dll files in path
	string s = "\\*.dll"; // only .dll endings
	dir = FindFirstFileA((path + s).c_str(), &fileData); // Notice: Unicode compatible version of FindFirstFile
	if (dir == INVALID_HANDLE_VALUE) //check if the dir opened successfully
	{
		status = false;
		goto EXIT;
	}

	do {

		fileName = fileData.cFileName;
		fullFileName = path + "\\" + fileName;

		// Load dynamic library
		HINSTANCE hDll = LoadLibraryA(fullFileName.c_str()); // Notice: Unicode compatible version of LoadLibrary
		if (!hDll) {
			errmsgs.addErrorMsg(CANNOT_LOAD_DLL_IDX, CANNOT_LOAD_DLL_MSG);
			status = false;
			goto EXIT;
		}

		// Get function pointer
		/* Should probably be done in battle*/
		/*
		getAlgoFunc = (GetAlgoFuncType)GetProcAddress(hDll, "GetAlgorithm");
		if (!getAlgoFunc) {
			errmsgs.addErrorMsg(CANNOT_LOAD_DLL_IDX, CANNOT_LOAD_DLL_MSG);
			status = false;
			goto EXIT;
		}
		*/

		dllList.push_back(hDll);
		//std::vector<IBattleshipGameAlgo *> players;
		//playersList.push_back(getAlgoFunc());

		//called once to notify player on his board
		/*

		//this section of code is no longer relevent, we will manage the 'setboards' and and competitions of diffrent players in 'war' function!!


		if (playerNumber == PLAYER_A)
		{
		algoA = getAlgoFunc();
		setBoard(playerNumber, board, algoA);
		//init_failed = !algoA->init(path); // init A_Board
		}
		else
		{
		algoB = getAlgoFunc();
		setBoard(playerNumber, board, algoB);
		//init_failed = !algoB->init(path);       // init B_Board
		}


		init_failed = 0;
		//call the players init function
		if (init_failed) {
		errmsgs.addErrorMsg(ALGO_INIT_FAIL_IDX, ALGO_INIT_FAIL_MSG);
		status = false;
		goto EXIT;
		}

		playerNumber+=1;

		*/

	} while (FindNextFileA(dir, &fileData) && playerNumber < 2); // Notice: Unicode compatible version of FindNextFile

EXIT:
	errmsgs.printErrorMsg();
	return status;
}

void ThreadManager::creatListOfGames()
{
	for (int i = 0; i < dllList.size(); i++)
	{
		for (int j = 0; j < dllList.size(); j++)
		{
			if (i != j)
				listOfGames.push_back(std::make_pair(i, j));
		}
	}

	/* DEBUG */
	INFO_PRINT("list of games is: \n");
	for (size_t i = 0; i < listOfGames.size(); i++)
	{
		INFO_PRINT("<%d,%d>\n", listOfGames[i].first, listOfGames[i].second);
	}
}

void ThreadManager::initial_numberOfRounds_numberOfPlayers_scoreTabel_playerRound()
{
	numberOfRounds = (dllList.size() - 1) * 2;
	numberOfPlayers = dllList.size();

	// init score tabel

	// size numberOfRounds + 1 because we allocate a round 0 where the scores are 0 (simplifies algorithm).
	scoreTabel.resize(numberOfRounds + 1);
	//scoreTabel = new std::vector<PlayerScore>[numberOfRounds + 1];
	//scoreTabel = new PlayerScore*[numberOfRounds + 1];
	for (size_t i = 0; i < numberOfRounds + 1; i++)
	{
		// Each vector is of the size of the amount of players
		scoreTabel[i].resize(numberOfPlayers);
		//scoreTabel[i] = new PlayerScore[numberOfPlayers];
		for (size_t j = 0; j < numberOfPlayers; j++)
		{
			if (i == 0) {
				scoreTabel[i][j].totalPointsFor = 0;
				scoreTabel[i][j].totalPointsAgainst = 0;
			}
			else {
				scoreTabel[i][j].totalPointsFor = -1;
				scoreTabel[i][j].totalPointsAgainst = -1;
			}

			scoreTabel[i][j].wins = 0;
			scoreTabel[i][j].losses = 0;
		}
	}

	playerRound = new std::atomic<int>[numberOfPlayers];
	for (size_t i = 0; i < numberOfPlayers; i++)
	{
		playerRound[i] = 1;
	}
	// Init rounds
}


