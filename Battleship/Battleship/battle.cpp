#include "battle.h"
#include <stdlib.h>
#include "ships.h"
#include "battle_utils.h"
#include "macros.h"
#include "error_class.h"
// This macro is to be used only from battle.cpp. Returned by Battle.attack() in case no more moves available for algo
#define BATTLE_OUT_OF_MOVES		(ALGO_OUT_OF_MOVES - 1)
#define VALID_INT_TOKENS  (0 < stoi(tokens[0]) && stoi(tokens[0]) < 11 && 0 < stoi(tokens[1]) && stoi(tokens[1]) < 11)

#define CANNOT_LOAD_DLL_MSG			"Cannot load dll: " + fullFileName + "\n"
#define CANNOT_LOAD_DLL_IDX			0

#define ALGO_INIT_FAIL_MSG			"Algorithm initialization failed for dll: " + fullFileName + "\n"
#define ALGO_INIT_FAIL_IDX			1


#define ALGO_INIT_FAILED			"Cannot load dll: " << fullFileName << endl
#define WRONG_PATH_IDX			1

#define NUM_OF_DLL_ERR_MSGS			2



// split,processLine,loadFromAttackFile methods are from Tirgul 3 , with some minor changes



void Battle::setWhosTurn(int turn)
{
	this->whosTurn = turn;
}

void Battle::setBoard(int player, const char** board, int numRows, int numCols, IBattleshipGameAlgo * algo)
{
	algo->setBoard(player, board, numRows, numCols);

	// Count number of squares for the player. TODO: do more efficiently?
	if (player)					//set Board to Player B
	{
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCols; j++)
			{
				if (isBelongToBoard(board[i][j]) == B_SQUARE)        
				{
					numOfSquareB += 1;
				}
			}
		}
	}
	
	else                                //set Board to Player A
	{
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCols; j++)
			{
				if (isBelongToBoard(board[i][j]) == A_SQUARE)
				{
					numOfSquareA += 1;
				}
			}
		}
	}

}

std::pair<int, int> Battle::attack(IBattleshipGameAlgo &algo)
{
	std::pair<int, int> attackPair;
	attackPair = algo.attack();
	attackPair.first -= 1;
	attackPair.second -= 1;

	return attackPair;
}

// Notify both algos on result of attack
void Battle::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	algoA->notifyOnAttackResult(player, row + 1, col + 1, result);
	algoB->notifyOnAttackResult(player, row + 1, col + 1, result);
}

bool Battle::init(const std::string & path)
{
	return false;
}

// Taken from recitation 5
bool Battle::loadDllFiles(const string& path, const Board &board) {

	HANDLE dir;
	WIN32_FIND_DATAA fileData; //data struct for file
	string fileName, fullFileName;
	int playerNumber = 0;
	bool init_failed;
	bool status = true;
	// define function of the type we expect
	typedef IBattleshipGameAlgo *(*GetAlgoFuncType)();
	GetAlgoFuncType getAlgoFunc;
	
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
		getAlgoFunc = (GetAlgoFuncType)GetProcAddress(hDll, "GetAlgorithm");
		if (!getAlgoFunc) {
			errmsgs.addErrorMsg(CANNOT_LOAD_DLL_IDX, CANNOT_LOAD_DLL_MSG);
			status = false;
			goto EXIT;
		}

		dllList.push_back(hDll);
		//players.push_back(getAlgoFunc());

		//called once to notify player on his board
		if (playerNumber == PLAYER_A)
		{
			algoA = getAlgoFunc();
			setBoard(playerNumber, (const char **)board.board, BOARD_SIZE, BOARD_SIZE, algoA);
			init_failed = !algoA->init(path); // init A_Board
		}
		else
		{
			algoB = getAlgoFunc();
			setBoard(playerNumber, (const char **)board.board, BOARD_SIZE, BOARD_SIZE, algoB);
			init_failed = !algoB->init(path);       // init B_Board
		}

	

		//call the players init function
		if (init_failed) {
			errmsgs.addErrorMsg(ALGO_INIT_FAIL_IDX, ALGO_INIT_FAIL_MSG);
			status = false;
			goto EXIT;
		}

		playerNumber+=1;

	} while (FindNextFileA(dir, &fileData) && playerNumber < 2); // Notice: Unicode compatible version of FindNextFile

EXIT:
	errmsgs.printErrorMsg();
	return status;
}



int Battle::War(const string &path, const Board &board)
{	

	
	//load the players algorithms
	if (!loadDllFiles(path, board ) )
		return false;

	//set each player board
/*	setWhosTurn(0);						 //   set turn A
	setBoard(this->whosTurn, (const char **)board.board, 10, 10, algoA);
	setWhosTurn(1);						//    set turn B
	setBoard(this->whosTurn, (const char **)board.board, 10, 10, algoB);
	
	// Init 
	algoA.init(path);
	algoB.init(path);

*/
	//set players attack vectors
	//loadFromAttackFile(fileParser.getAttackAFileName(), 0);
	//loadFromAttackFile(fileParser.getAttackBFileName(), 1);

	// Deep copy ship lists
	vector<Ship*> shipListA = deepCopyShipPointerVector(board.shipListA);
	int pointsA = 0;
	vector<Ship*> shipListB = deepCopyShipPointerVector(board.shipListB);
	int pointsB = 0;

	//war!!
	setWhosTurn(0);						 //   set turn A
	bool twoPlayersOutOfPlays = false, alreadyGotHit = false, HitCorrectTarget = false, playerAOutOfPlays = false, playerBOutOfPlays = false;
	int indexA = 0, indexB = 0, pointsReceived = 0;
	int whoGotHit;
	int x = -1, y = -1;
	std::pair<int, int> attackPair;
	AttackResult attackResult;

	display.printBoard(board);
	
	// Make all prints after board 
	display.gotoxy(0, BOARD_DISPLAY_OFFSET_Y + BOARD_SIZE + 2);
	while ( this->numOfSquareA > 0 && this->numOfSquareB > 0  && !twoPlayersOutOfPlays)
	{		
		if (this->whosTurn)			//player B
		{
			attackResult = AttackResult::Miss;
			// Get attack move from player B
			attackPair = attack(*algoB);
			// If player B out of moves
			if (attackPair.first == BATTLE_OUT_OF_MOVES) {
				playerBOutOfPlays = true;
				setWhosTurn((this->whosTurn + 1) % 2);
				twoPlayersOutOfPlays = (playerAOutOfPlays && playerBOutOfPlays);
			}
			
			else
			{
				x = attackPair.first;
				y = attackPair.second;
				
				DEBUG_PRINT("B shot A at <%d,%d>\n", x, y);
				whoGotHit = isBelongToBoard(board.board[x][y]);

				if (whoGotHit == NO_SQUARE)                // miss
				{
					HitCorrectTarget = false;
					attackResult = AttackResult::Miss;
					if (!IS_SQUARE_DOWN(board.board[x][y]))
					{
						board.board[x][y] = SQUARE_BOMBED_MISS_SYMBOL;
					}
					
				}
				else if (whoGotHit == A_SQUARE)		  // player B Hit player A
				{
					attackResult = AttackResult::Miss;

					alreadyGotHit = (IS_SQUARE_DOWN(board.board[x][y]));
					if (!alreadyGotHit)
					{

						pointsReceived = shootShip(Point(x, y), shipListA);
						pointsB += pointsReceived;
						DEBUG_PRINT("B shot A at point <%d,%d>. Now B has %d points ", x, y, pointsB);
						DEBUG_PRINT("Ship hit is %c\n", board.board[x][y]);

						attackResult = AttackResult::Hit;
						this->numOfSquareA -= 1;
						board.board[x][y] = SQUARE_BOMBED_HIT_SYMBOL;   //mark as hit
						HitCorrectTarget = true;

						// If ship sank
						if (pointsReceived > 0)
						{
							DEBUG_PRINT("Ship sank!\n");
							attackResult = AttackResult::Sink;
						}
					}

					
				}

				else							//B Hit himself!! // TODO: notify a hit or a miss? notifying as a miss for now
				{
					alreadyGotHit = (IS_SQUARE_DOWN(board.board[x][y]));
					if (!alreadyGotHit)
					{
						

						pointsReceived = shootShip(Point(x, y), shipListB);
						pointsA += pointsReceived;

						DEBUG_PRINT("B shot B at point <%d,%d>. Now A has %d points ", x, y, pointsA);
						DEBUG_PRINT("Ship hit is %c\n", board.board[x][y]);

						// Notify as miss because did not hit correct ship 
						attackResult = AttackResult::Miss;
						this->numOfSquareB -= 1;
						board.board[x][y] = SQUARE_BOMBED_HIT_SYMBOL;   //mark as hit
						HitCorrectTarget = false;

						
						if (pointsReceived > 0)
						{
							DEBUG_PRINT("Ship sank!\n");
							
						}
					}
				}

				notifyOnAttackResult(PLAYER_B, x, y, attackResult);
				//if ( (!playerAOutOfPlays && !HitCorrectTarget )|| alreadyGotHit)
				if (!playerAOutOfPlays && attackResult == AttackResult::Miss)
					setWhosTurn((this->whosTurn + 1) % 2);          //change turn to next player if possibale (to player A)
				
				alreadyGotHit = false;
			}
		}

		else                       //player A
		{
			attackPair = attack(*algoA);
			
			// If player B out of moves
			if (attackPair.first == BATTLE_OUT_OF_MOVES) {
				playerAOutOfPlays = true;
				setWhosTurn((this->whosTurn + 1) % 2);
				twoPlayersOutOfPlays = (playerAOutOfPlays && playerBOutOfPlays);
			}

			else
			{

			x = attackPair.first;
			y = attackPair.second;
			
			DEBUG_PRINT("A shot B at <%d,%d>\n", x, y);
			whoGotHit = isBelongToBoard(board.board[x][y]);
			if (whoGotHit == NO_SQUARE )                // miss
			{
				HitCorrectTarget = false;
				attackResult = AttackResult::Miss;
				if (!IS_SQUARE_DOWN(board.board[x][y]))
				{
					board.board[x][y] = SQUARE_BOMBED_MISS_SYMBOL;
				}
			}
			else if (whoGotHit == B_SQUARE)		  // player A Hit player B
			{
				attackResult = AttackResult::Hit;
				alreadyGotHit = (IS_SQUARE_DOWN(board.board[x][y]));

				if (!alreadyGotHit)
				{
					
					pointsReceived = shootShip(Point(x, y), shipListB);
					pointsA += pointsReceived;
					DEBUG_PRINT("A shot B at point <%d,%d>. Now A has %d points ", x, y, pointsA);
					DEBUG_PRINT("Ship hit is %c\n", board.board[x][y]);

					// Need to notify
					this->numOfSquareB -= 1;
					board.board[x][y] = SQUARE_BOMBED_HIT_SYMBOL;   //mark as hit
				}

				// If ship sank
				if (pointsReceived > 0)
				{
					DEBUG_PRINT("Ship sank!\n");
					attackResult = AttackResult::Sink;
				}

				
			}
			else               //A Hit himself!!
			{
				alreadyGotHit = (IS_SQUARE_DOWN(board.board[x][y]));
				if (!alreadyGotHit)
				{
					// If ship sank - notify as miss because did not hit correct ship 
					attackResult = AttackResult::Miss;
					pointsReceived = shootShip(Point(x, y), shipListA);
					pointsB += pointsReceived;

					DEBUG_PRINT("A shot A at point <%d,%d>. Now B has %d points ", x, y, pointsB);
					DEBUG_PRINT("Ship hit is %c\n", board.board[x][y]);

					this->numOfSquareA -= 1;

					board.board[x][y] = SQUARE_BOMBED_HIT_SYMBOL;   //mark has hit
					HitCorrectTarget = false;
					
					if (pointsReceived > 0)
					{
						DEBUG_PRINT("Ship sank!\n");
						
					}
				}
			}

			indexA += 1;
			notifyOnAttackResult(PLAYER_A, x, y, attackResult);
			//if ( (!playerBOutOfPlays && !HitCorrectTarget ) || alreadyGotHit)
			if (!playerBOutOfPlays && attackResult == AttackResult::Miss)
				setWhosTurn((this->whosTurn + 1) % 2);          //change turn to next player if possibale and the player missed (to player B)
			
			alreadyGotHit = false;

		    }
	    }

		// Update board
		display.updateSquare(x, y, board.board[x][y]);
	}
	
	
	//game is over, print winners if there are
	if (this->numOfSquareB == 0)
	{
		std::cout << "Player A won" << std::endl; 
	}
	else if(this->numOfSquareA == 0)
	{
		std::cout << "Player B won" << std::endl;
	}
	
	std::cout << "Points:"    << std::endl;
	std::cout << "Player A: " << to_string(pointsA) << std::endl;
	std::cout << "Player B: " << to_string(pointsB) << std::endl;

	

	// Delete cloned ship lists
	for (size_t i = 0; i < shipListA.size(); i++)
	{
		delete shipListA[i];
	}

	for (size_t i = 0; i < shipListB.size(); i++)
	{
		delete shipListB[i];
	}


	return 0;
}









