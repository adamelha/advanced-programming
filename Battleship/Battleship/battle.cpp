#include "battle.h"
#include <stdlib.h>
#include "ships.h"

#define VALID_INT_TOKENS  (0 < stoi(tokens[0]) && stoi(tokens[0]) < 11 && 0 < stoi(tokens[1]) && stoi(tokens[1]) < 11)

//#define IS_CHAR_BELONG_TO_A_BOARD     board[i][j] == 'B' || board[i][j] == 'P' || board[i][j] == 'M' || board[i][j]=='D'  
//#define IS_CHAR_BELONG_TO_B_BOARD     board[i][j] == 'b' || board[i][j] == 'p' || board[i][j] == 'm' || board[i][j]=='d'  

enum {NO_SQUARE = 0, A_SQUARE = 1, B_SQUARE = 2};

// this method from Tirgul 3 

static int isBelongToBoard(const char c ) 
{
	if (c == 'B' || c == 'P' || c == 'M' || c == 'D' )         //A Square
		return A_SQUARE;
	else if (c == 'b' || c == 'p' || c == 'm' || c == 'd')     // B sqare
		return B_SQUARE;
	else {
		return NO_SQUARE;
	}
}
static std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	std::stringstream strSt(s);
	std::string item;

	while (std::getline(strSt, item, delim)) //we could ommit the 'std::' notion from here.
	{
		elems.push_back(item); //push_back is a function of vector class, it inserted to the back of the vector.
	}

	return elems;
}


/*void Battle::setLineOrColumn(char board[][BOARD_SIZE], int size , Point* p , bool isVertical, char symbol )
{
	if(isVertical)
	{
		for(int i =0 ; i < size ; i++)
			board[p->x][p->y + i ] = symbol;	
	}
	
	else
	{
		for (int i = 0; i < size; i++)
			board[p->x + i][p->y] = symbol;	
	}
}*/

void Battle::setAttacker(string attackStr, int whosTurn)
{
	vector<string> attacker = split(attackStr, '\n');
	if (whosTurn)		//B attack 
	{
		for(int i = 0; i < attacker.size(); i++)
		{
			
		}

	}
	else                //A attack
	{



	}
}

void Battle::processLine(const string & line, int whosTurn)
{
	vector<string> tokens = split(line, ',');
	if (tokens.size() != 2 || !VALID_INT_TOKENS) //TODO: improve define
	{
		return;
	}
	pair<int, int> aPair(stoi(tokens[0]) - 1, stoi(tokens[1]) - 1);
	if (whosTurn)								// B
		B_Atacker.push_back(aPair);
	else
		A_Atacker.push_back(aPair);
	
	
}

void Battle::loadFromAttackFile(const string & attackPath, int whosTurn)
{
	if(whosTurn)
		this->B_Atacker.clear();		//clears the vector
	else
		this->A_Atacker.clear();
	ifstream fin(attackPath);			//creating an ifstream object and opening file in path attackPath 
	string line;
	while (getline(fin, line))		   //getline returns null if we get to end of file.
	{
		this->processLine(line, whosTurn);
	}
}



void Battle::setFilePath(string path)
{
	this->filesPath = path;
}

void Battle::setWhosTurn(int turn)
{
	this->whosTurn = turn;
}

void Battle::setBoard(const char** board, int numRows, int numCols)
{	
	if (this->whosTurn)					//set Board to Player B
	{
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCols; j++)
			{
				if (isBelongToBoard(board[i][j]) == B_SQUARE)        
				{
					this->B_Board[i][j] = board[i][j];
					numOfSquareB += 1;
				}
				else
					this->B_Board[i][j] = ' ';       // initialize board to 
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
					this->A_Board[i][j] = board[i][j];
					numOfSquareA += 1;
				}
				else
					this->A_Board[i][j] = ' ';
			}
		}
	}



}

std::pair<int, int> Battle::attack()
{
	return std::pair<int, int>();
}

void Battle::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
}

int Battle::War(const FileParser &fileParser, const Board &board)
{
	//initialize full const board for method setBoard( memory allocation included )
	char fullBoard[BOARD_SIZE][BOARD_SIZE];
	for (int i = 0 ; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
			fullBoard[i][j] = board.getCharFromBoard(i, j);
	}
	char ** constFullBoard = (char**)malloc (BOARD_SIZE * sizeof(char*));
	for (int i = 0; i < BOARD_SIZE; i++)
		constFullBoard[i] = (char*)malloc (BOARD_SIZE * sizeof(char));
	for (int i = 0 ; i < BOARD_SIZE; i++)
		//constFullBoard[i] = fullBoard[i];
		memcpy(constFullBoard[i], fullBoard[i], BOARD_SIZE);

	//set each player board
	setWhosTurn(0);						 //   set turn A
	setBoard((const char **)constFullBoard, 10, 10);
	setWhosTurn(1);						//    set turn B
	setBoard((const char **)constFullBoard, 10, 10);
	
	//set players attack vectors
	loadFromAttackFile(fileParser.getAttackAFileName(), 0);
	loadFromAttackFile(fileParser.getAttackBFileName(), 1);

	// Deep copy ship lists
	vector<Ship*> shipListA = deepCopyShipPointerVector(board.shipListA);
	int pointsA = 0;
	vector<Ship*> shipListB = deepCopyShipPointerVector(board.shipListB);
	int pointsB = 0;
	//war!!
	setWhosTurn(0);						 //   set turn A
	bool twoPlayersOutOfPlays = false , alreadyGotHit = false ,HitCorrectTarget = false;
	int indexA = 0, indexB = 0;
	int whoGotHit;
	int x, y;
	int total = this->B_Atacker.size() + this->A_Atacker.size();
	while ( this->numOfSquareA > 0 && this->numOfSquareB > 0  && !twoPlayersOutOfPlays)
	{
		if (this->whosTurn)			//player B
		{
			x = this->B_Atacker[indexB].first;
			y = this->B_Atacker[indexB].second;
			whoGotHit = isBelongToBoard(board.board[x][y]);   

			if (whoGotHit == NO_SQUARE )                // miss
			{
				HitCorrectTarget = false;
				//need to notify for next assignments
			}
			else if (whoGotHit == A_SQUARE)		  // player B Hit player A
			{
				alreadyGotHit = (this->A_Board[x][y] == SQUARE_DOWN_SYMBOL);
				if (!alreadyGotHit)
				{
					this->numOfSquareA -= 1;
					twoPlayersOutOfPlays = (total < indexA + indexB + 1);
					this->A_Board[x][y] = SQUARE_DOWN_SYMBOL;   //mark as hit
					HitCorrectTarget = true;

					pointsB += shootShip(Point(x, y), shipListA);
					DEBUG_PRINT("B shot A at point <%d,%d>. Now B has %d points ", x, y, pointsB);
					DEBUG_PRINT("Ship hit is %c\n", board.board[x][y]);
				}
			}
			else							//B Hit himself!!
			{
				alreadyGotHit = (this->B_Board[x][y] == SQUARE_DOWN_SYMBOL);
				if (!alreadyGotHit)
				{
					this->numOfSquareB -= 1;
					twoPlayersOutOfPlays = (total < indexA + indexB + 1);
					this->B_Board[x][y] = SQUARE_DOWN_SYMBOL;   //mark as hit
					HitCorrectTarget = false;

					pointsA += shootShip(Point(x, y), shipListB);
					DEBUG_PRINT("B shot B at point <%d,%d>. Now A has %d points ", x, y, pointsA);
					DEBUG_PRINT("Ship hit is %c\n", board.board[x][y]);
				}
			}

			indexB += 1;
			if (indexA < this->A_Atacker.size() && !HitCorrectTarget)
				setWhosTurn((this->whosTurn + 1) % 2) ;          //change turn to next player if possibale (to player A)
		}


		else                       //player A
		{
			x = this->A_Atacker[indexA].first;
			y = this->A_Atacker[indexA].second;
			whoGotHit = isBelongToBoard(fullBoard[x][y]);
			if (whoGotHit == NO_SQUARE )                // miss
			{
				HitCorrectTarget = false;
				//need to notify for next assignments
			}
			else if (whoGotHit == B_SQUARE)		  // player A Hit player B
			{
				alreadyGotHit = (this->B_Board[x][y] == SQUARE_DOWN_SYMBOL);
				if (!alreadyGotHit)
				{
					// Need to notify
					this->numOfSquareB -= 1;
					twoPlayersOutOfPlays = (total < indexA + indexB + 1);
					this->B_Board[x][y] = SQUARE_DOWN_SYMBOL;   //mark has hit
					HitCorrectTarget = true;
					pointsA += shootShip(Point(x, y), shipListB);
					DEBUG_PRINT("A shot B at point <%d,%d>. Now A has %d points ", x, y, pointsA);
					DEBUG_PRINT("Ship hit is %c\n", board.board[x][y]);
				}
			}
			else               //A Hit himself!!
			{
				alreadyGotHit = (this->A_Board[x][y] == SQUARE_DOWN_SYMBOL);
				if (!alreadyGotHit)
				{
					this->numOfSquareA -= 1;
					twoPlayersOutOfPlays = (total < indexA + indexB + 1);
					this->A_Board[x][y] = SQUARE_DOWN_SYMBOL;   //mark has hit
					HitCorrectTarget = false;

					pointsB += shootShip(Point(x, y), shipListA);
					DEBUG_PRINT("A shot A at point <%d,%d>. Now B has %d points ", x, y, pointsB);
					DEBUG_PRINT("Ship hit is %c\n", board.board[x][y]);
				}
			}

			indexA += 1;
			if (indexB < this->B_Atacker.size() && !HitCorrectTarget)
				setWhosTurn((this->whosTurn + 1) % 2);          //change turn to next player if possibale and the player missed (to player B)
		}
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

	std::cout << "Player A: " << to_string(pointsA) << std::endl;
	std::cout << "Player B: " << to_string(pointsB) << std::endl;

	//free allocation
	for (size_t i  = 0; i < BOARD_SIZE; i++)
	{
		free(constFullBoard[i]);
	}

	// Delete cloned ship lists
	for (size_t i = 0; i < shipListA.size(); i++)
	{
		delete shipListA[i];
	}

	for (size_t i = 0; i < shipListB.size(); i++)
	{
		delete shipListB[i];
	}

	free(constFullBoard);

	return 0;
}









