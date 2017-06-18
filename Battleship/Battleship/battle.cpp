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

#define NUM_OF_DLL_ERR_MSGS			2



void Battle::setWhosTurn(int turn)
{
	this->whosTurn = turn;
}



//void Battle::setBoard(int player, const char** board, int numRows, int numCols, IBattleshipGameAlgo * algo)
void Battle::setBoard(int player, const Board &board, IBattleshipGameAlgo * algo)
{
	algo->setPlayer(player);
	algo->setBoard(board);
	
	// Count number of squares for the player. TODO: do more efficiently?
	if (player)					//set Board to Player B
	{
		for (int x = 0; x < board.rows(); x++)
		{
			for (int y = 0; y < board.cols(); y++)
			{
				for (int z = 0; z < board.depth(); z++)
				{
					if (isBelongToBoard(board.charAt(Coordinate(x, y, z))) == B_SQUARE)
					{
						numOfSquareB += 1;
					}
				}
				
			}
		}
	}
	
	else                                //set Board to Player A
	{
		for (int x = 0; x < board.rows(); x++)
		{
			for (int y = 0; y < board.cols(); y++)
			{
				for (int z = 0; z < board.depth(); z++)
				{
					if (isBelongToBoard(board.charAt(Coordinate(x, y, z))) == A_SQUARE)
					{
						numOfSquareA += 1;
					}
				}
				
			}
		}
	}
	
}

Coordinate Battle::attack(IBattleshipGameAlgo &algo)
{

	Coordinate attackPoint = algo.attack();

	attackPoint.row -= 1;
	attackPoint.col -= 1;
	attackPoint.depth -= 1;

	return attackPoint;
}

// Notify both algos on result of attack
void Battle::notifyOnAttackResult(int player, Coordinate p, AttackResult result)
{

	// Update point according to interface
	p.row += 1;
	p.col += 1;
	p.depth += 1;

	algoA->notifyOnAttackResult(player, p, result);
	algoB->notifyOnAttackResult(player, p, result);
}

bool Battle::init(const std::string & path)
{
	return false;
}

// Taken from recitation 5
/*
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
		//std::vector<IBattleshipGameAlgo *> players;
		playersList.push_back(getAlgoFunc());

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
		
		

	} while (FindNextFileA(dir, &fileData) && playerNumber < 2); // Notice: Unicode compatible version of FindNextFile


EXIT:
	errmsgs.printErrorMsg();
	return status;
}

*/

BattleScore Battle::War(const Board &board,  IBattleshipGameAlgo* _algoA, IBattleshipGameAlgo* _algoB)
{	

	
	//load the players algorithms
	//if (!loadDllFiles(path, board) )
	//	return false;
	BattleScore battleScore;
	algoA = _algoA;
	algoB = _algoB;
	setBoard(player_A, board, algoA);  // setBoard to player A
	setBoard(player_B, board, algoB);	// setBoard to player B

	// Create local copy of board
	// TODO - either free when finished or use smar pointer
	char ***localBoard = alloc3dArray<char>(board.rows(), board.cols(), board.depth());
	copy3dArray(localBoard, board.board, board.rows(), board.cols(), board.depth());

	// Deep copy ship lists
	vector<Ship*> shipListA = deepCopyShipPointerVector(board.shipListA);
	int pointsA = 0;
	vector<Ship*> shipListB = deepCopyShipPointerVector(board.shipListB);
	int pointsB = 0;
	// each player potential number of moves under the assumptions: 1)player don't attack is own ships 2)that each player attacks a square at most once(whitch is not is own)
	int NumnerOfPotentialMovesA = (board.rows() * board.cols() * board.depth() ) - numOfSquareA;
	int NumnerOfPotentialMovesB = (board.rows() * board.cols() * board.depth() ) - numOfSquareB;

	//war!!
	setWhosTurn(player_A);						 //   set turn A
	bool twoPlayersOutOfPlays = false, alreadyGotHit = false, HitCorrectTarget = false, playerAOutOfPlays = false, playerBOutOfPlays = false;
	int pointsReceived = 0;
	int whoGotHit;
	int x = -1, y = -1, z = -1;
	int roundCounter = 0;
	Point attackPoint;
	AttackResult attackResult;

	
	// Main loop
	while ( this->numOfSquareA > 0 && this->numOfSquareB > 0  && !twoPlayersOutOfPlays)
	{	
		roundCounter++;
		if (this->whosTurn)			//player B
		{

			if (NumnerOfPotentialMovesA <= 0 && NumnerOfPotentialMovesB <= 0)     //if both players out of moves(bug in algo which lead to draw)
			{
				battleScore.winner = Winner::Tie;
				break;

			} 

			attackResult = AttackResult::Miss;
			// Get attack move from player B
			attackPoint = attack(*algoB);

			// If player B out of moves
			if (attackPoint.row == BATTLE_OUT_OF_MOVES) {
				playerBOutOfPlays = true;
				setWhosTurn((this->whosTurn + 1) % 2);
				twoPlayersOutOfPlays = (playerAOutOfPlays && playerBOutOfPlays);
			}
			
			else
			{
				x = attackPoint.row;
				y = attackPoint.col;
				z = attackPoint.depth;
				
				
				DEBUG_PRINT("B shot A at <%d,%d,%d>\n", x, y, z);
				whoGotHit = isBelongToBoard(localBoard[x][y][z]);

				if (whoGotHit == NO_SQUARE)                // miss
				{
					HitCorrectTarget = false;
					attackResult = AttackResult::Miss;
					if (!IS_SQUARE_DOWN(localBoard[x][y][z]))
					{
						localBoard[x][y][z] = SQUARE_BOMBED_MISS_SYMBOL;
					}
					
				}
				else if (whoGotHit == A_SQUARE)		  // player B Hit player A
				{
					attackResult = AttackResult::Miss;

					alreadyGotHit = (IS_SQUARE_DOWN(localBoard[x][y][z]));
					if (!alreadyGotHit)
					{

						pointsReceived = shootShip(Point(x, y, z), shipListA);
						pointsB += pointsReceived;
						DEBUG_PRINT("B shot A at point <%d,%d,%d>. Now B has %d points ", x, y, z, pointsB);
						DEBUG_PRINT("Ship hit is %c\n", localBoard[x][y][z]);

						attackResult = AttackResult::Hit;
						this->numOfSquareA -= 1;
						localBoard[x][y][z] = SQUARE_BOMBED_HIT_SYMBOL;   //mark as hit
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
					alreadyGotHit = (IS_SQUARE_DOWN(localBoard[x][y][z]));
					if (!alreadyGotHit)
					{
						

						pointsReceived = shootShip(Point(x, y, z), shipListB);
						pointsA += pointsReceived;

						DEBUG_PRINT("B shot B at point <%d,%d,%d>. Now A has %d points ", x, y, z, pointsA);
						DEBUG_PRINT("Ship hit is %c\n", localBoard[x][y][z]);

						// Notify as miss because did not hit correct ship 
						attackResult = AttackResult::Miss;
						this->numOfSquareB -= 1;
						localBoard[x][y][z] = SQUARE_BOMBED_HIT_SYMBOL;   //mark as hit
						HitCorrectTarget = false;

						
						if (pointsReceived > 0)
						{
							DEBUG_PRINT("Ship sank!\n");
							
						}
					}
				}

				notifyOnAttackResult(PLAYER_B, attackPoint, attackResult);
				//if ( (!playerAOutOfPlays && !HitCorrectTarget )|| alreadyGotHit)
				if (!playerAOutOfPlays && attackResult == AttackResult::Miss)
					setWhosTurn((this->whosTurn + 1) % 2);          //change turn to next player if possibale (to player A)
				
				alreadyGotHit = false;
			}
			NumnerOfPotentialMovesB -= 1;
		}

		else                       //player A
		{
			
			if (NumnerOfPotentialMovesA <= 0 && NumnerOfPotentialMovesB <= 0)     //if both players out of moves(bug in algo which lead to draw)
			{
				battleScore.winner = Winner::Tie;
				break;

			}
			
			attackPoint = attack(*algoA);
			// If player B out of moves
			if (attackPoint.row == BATTLE_OUT_OF_MOVES) {
				playerAOutOfPlays = true;
				setWhosTurn((this->whosTurn + 1) % 2);
				twoPlayersOutOfPlays = (playerAOutOfPlays && playerBOutOfPlays);
			}

			else
			{

				x = attackPoint.row;
				y = attackPoint.col;
				z = attackPoint.depth;

				DEBUG_PRINT("A shot B at <%d,%d,%d>\n", x, y, z);
				whoGotHit = isBelongToBoard(localBoard[x][y][z]);
				if (whoGotHit == NO_SQUARE )                // miss
				{
					HitCorrectTarget = false;
					attackResult = AttackResult::Miss;
					if (!IS_SQUARE_DOWN(localBoard[x][y][z]))
					{
						localBoard[x][y][z] = SQUARE_BOMBED_MISS_SYMBOL;
					}
				}
				else if (whoGotHit == B_SQUARE)		  // player A Hit player B
				{
					attackResult = AttackResult::Hit;
					alreadyGotHit = (IS_SQUARE_DOWN(localBoard[x][y][z]));

					if (!alreadyGotHit)
					{
					
						pointsReceived = shootShip(Point(x, y, z), shipListB);
						pointsA += pointsReceived;
						DEBUG_PRINT("A shot B at point <%d,%d,%d>. Now A has %d points ", x, y, z, pointsA);
						DEBUG_PRINT("Ship hit is %c\n", localBoard[x][y][z]);

					// Need to notify
						this->numOfSquareB -= 1;
						localBoard[x][y][z] = SQUARE_BOMBED_HIT_SYMBOL;   //mark as hit
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
					alreadyGotHit = (IS_SQUARE_DOWN(localBoard[x][y][z]));
					if (!alreadyGotHit)
					{
						// If ship sank - notify as miss because did not hit correct ship 
						attackResult = AttackResult::Miss;
						pointsReceived = shootShip(Point(x, y, z), shipListA);
						pointsB += pointsReceived;

						DEBUG_PRINT("A shot A at point <%d,%d,%d>. Now B has %d points ", x, y, z, pointsB);
						DEBUG_PRINT("Ship hit is %c\n", localBoard[x][y][z]);

						this->numOfSquareA -= 1;

						localBoard[x][y][z] = SQUARE_BOMBED_HIT_SYMBOL;   //mark has hit
						HitCorrectTarget = false;
					
						if (pointsReceived > 0)
						{
							DEBUG_PRINT("Ship sank!\n");
						
						}
					}
				}

				notifyOnAttackResult(PLAYER_A, attackPoint, attackResult);
				//if ( (!playerBOutOfPlays && !HitCorrectTarget ) || alreadyGotHit)
				if (!playerBOutOfPlays && attackResult == AttackResult::Miss)
					setWhosTurn((this->whosTurn + 1) % 2);          //change turn to next player if possibale and the player missed (to player B)
			
				alreadyGotHit = false;

		    }
	    
			NumnerOfPotentialMovesA -= 1;       
		}

	}
	
	
	//game is over, print winners if there are
	printMutex.lock();
	if (this->numOfSquareB == 0)
	{
		std::cout << "Player A won" << std::endl; 
		battleScore.winner = Winner::PlayerA;
	}
	else if(this->numOfSquareA == 0)
	{
		std::cout << "Player B won" << std::endl;
		battleScore.winner = Winner::PlayerB;
	}
	else
	{
		std::cout << "Draw" << std::endl;
	}

	
	std::cout << "Points:"    << std::endl;
	std::cout << "Player A: " << to_string(pointsA) << std::endl;
	std::cout << "Player B: " << to_string(pointsB) << std::endl;

	printMutex.unlock();

	DEBUG_PRINT("Game took %d rounds to complete\n", roundCounter);
	

	// Delete cloned ship lists
	for (size_t i = 0; i < shipListA.size(); i++)
	{
		delete shipListA[i];
	}

	for (size_t i = 0; i < shipListB.size(); i++)
	{
		delete shipListB[i];
	}
	// in case of a draw no points will be counted as pointed out in forum
	if (battleScore.winner == Winner::Tie)       
	{
		battleScore.playerAPoints = 0;
		battleScore.playerBPoints = 0;

	}
	else
	{
		battleScore.playerAPoints = pointsA;
		battleScore.playerBPoints = pointsB;
	}
	

	return battleScore;
}









