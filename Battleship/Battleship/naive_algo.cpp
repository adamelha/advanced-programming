#include "naive_algo.h"
#include "macros.h"

#define isPointOnBoard(i , j)  ( i < BOARD_SIZE && i > -1 && j < BOARD_SIZE && j > -1 )

NaiveAlgo::NaiveAlgo()
{
	// Init board to be array of char pointers, each pointing to a row in _board matrix (avoids dynamic allocation)

	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		board[i] = &(_board[i][0]);
	}
}


bool NaiveAlgo::isPointPartOfShip(int x, int y)
{
	return	board[x][y] == 'b'
		|| board[x][y] == 'B'
		|| board[x][y] == 'p'
		|| board[x][y] == 'P'
		|| board[x][y] == 'm'
		|| board[x][y] == 'M'
		|| board[x][y] == 'd'
		|| board[x][y] == 'D';
}
//changes unvalid points near a point with player ship
void NaiveAlgo::changeEnvalopPointsToFalse(bool arr[][BOARD_SIZE], int x, int y)
{
	if( isPointPartOfShip(x, y) )
	{
		arr[x][y] = false;
		if (isPointOnBoard(x + 1, y))
			arr[x + 1][y] = false;
		if (isPointOnBoard(x, y + 1))
			arr[x][y + 1] = false;
		if (isPointOnBoard(x, y - 1))
			arr[x][y - 1] = false;
		if (isPointOnBoard(x - 1, y))
			arr[x - 1][y] = false;

	}
}

// Gal, here you must call the initializer! I don't think you can use ship list as an argument.
// Must create it here!
bool NaiveAlgo::init(const std::string& path)
{
	return true;
}

Point NaiveAlgo::getNextPotentialPoint()
{
	return Point();
}

Point * NaiveAlgo::potentialLegalMoves()
{
	return nullptr;
}

int NaiveAlgo::getPotentialMovesSize()
{
	return potentialMovesSize;
}

void NaiveAlgo::setBoard(int player, const char ** board, int numRows, int numCols)
{
	squareType_e sqaretTypeIs;
	if (player)	//set Board to Player B
	{
		sqaretTypeIs = B_SQUARE;
	}
	else //set Board to Player A
	{
		sqaretTypeIs = A_SQUARE;
	}

	for (int x = 0; x < numRows; x++)
	{
		for (int y = 0; y < numCols; y++)
		{
			if ( isBelongToBoard(board[x][y]) == sqaretTypeIs)
			{
				this->board[x][y] = board[x][y];
				this->numOfSqares += 1;
			}
			else
				this->board[x][y] = ' ';       // initialize board to 
		}
	}
}

std::pair<int, int> NaiveAlgo::attack()
{
	int curr = this->nextPointIndex;
	this->nextPointIndex += 1;
	if (this->nextPointIndex < getPotentialMovesSize() )
		return std::pair<int, int>(potentialMoves[curr].x , potentialMoves[curr].y);
	return std::pair<int, int>(-1,-1);
}

void NaiveAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{

}


//changes unvalid points near a point with player ship

//updated two dimientional isPointLegal[10[10],detemine which point (x,y) is valid=> isPointLegal[x][y] = true, else isPointLegal[x][y] = false
void NaiveAlgo::initIsPointLegal()
{
	memset(isPointLegal, true, sizeof(isPointLegal[0][0]) * BOARD_SIZE * BOARD_SIZE);   //all initialized to true

	for(int x = 0 ; x < BOARD_SIZE; x++)
	{
		for (int y = 0 ; y < BOARD_SIZE; y++)
		{
			changeEnvalopPointsToFalse( isPointLegal , x , y);
		}
	}
}


//potential Moves is an array with naiveAlgo attack moves by order!
void NaiveAlgo::initPotentialMoves()
{
	int index = 0;
	initIsPointLegal();
	for (int i = 0 ; i < BOARD_SIZE; i++)
	{
		for (int j =0 ; j < BOARD_SIZE; j++)
		{
			if(isPointLegal[i][j])
			{
				potentialMoves[index] = Point(i, j);
				index += 1;
				potentialMovesSize += 1;
			}

		}
	}

	//memset(array, 0, sizeof(array[0][0]) * m * n);

}
