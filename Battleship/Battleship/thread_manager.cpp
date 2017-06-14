#include "thread_manager.h"


// Thread that should take one game from the list, instantiate the algorithms and and manage the game
void ThreadManager::threadGameFunc()
{
	while( true )  // as long there are gamesto be played, play
	{ 
	int indexOfGame = atomicCounter.fetch_add(1, std::memory_order_relaxed);
	if (indexOfGame >= listOfGames.size())
		break;

	IBattleshipGameAlgo * firstPlayer  =  ( (GetAlgoFuncType)GetProcAddress(dllList[listOfGames[indexOfGame].first], "GetAlgorithm") )  () ; 
	IBattleshipGameAlgo * secondPlayer =   ((GetAlgoFuncType)GetProcAddress(dllList[listOfGames[indexOfGame].second], "GetAlgorithm") ) () ;
	Battle battle(player_A);
	battle.War(board, firstPlayer, secondPlayer);
	}
	// Print is not thread safe
	cout << "yalla" << endl; 
	//DEBUG_PRINT("%d\n", indexOfGame);
}


ThreadManager::ThreadManager(const string& _path, const Board& _board, int _numberOfThreads) : path(_path), board(_board), numberOfThreads(_numberOfThreads)
{
	threadList.resize(numberOfThreads); 
	loadDllFiles();
	creatListOfGames();

}

// This methods creates and runs the threads that call Battle::war() which will create instances of the 2 algos it's supposed to use

status_t ThreadManager::run()
{

	// Create threads to run game
	for (size_t i = 0; i < numberOfThreads; i++)
	{
		threadList[i] = std::thread(&ThreadManager::threadGameFunc, this);
	}

	// Here main thread should do his work printing scores
	while (1) {}



	// wait for threads to finish - not necessary here, just an example
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
}

