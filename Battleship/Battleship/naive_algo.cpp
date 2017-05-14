#include "naive_algo.h"
#include "macros.h"
#include "battle_utils.h"

#define isPointOnBoard(i , j)  ( i < BOARD_SIZE && i > -1 && j < BOARD_SIZE && j > -1 )

NaiveAlgo::NaiveAlgo()
{
	// Init board to be array of char pointers, each pointing to a row in _board matrix (avoids dynamic allocation)

	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		board[i] = &(_board[i][0]);
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
	squareType_e st;
	if (player)	//set Board to Player B
	{
		st = B_SQUARE;
	}
	else //set Board to Player A
	{
		st = A_SQUARE;
	}

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (isBelongToBoard(board[i][j]) == B_SQUARE)
			{
				this->board[i][j] = board[i][j];
				//numOfSquareB += 1;
			}
			else
				this->board[i][j] = ' ';       // initialize board to 
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
void changeEnvalopPointsToFalse(bool arr[][BOARD_SIZE] , int i ,int j)
{
	arr[i][j] = false;
	if (isPointOnBoard(i + 1, j))
		arr[i + 1][j] = false;
	if (isPointOnBoard(i , j + 1 ))
		arr[i][j + 1] = false;
	if (isPointOnBoard(i , j - 1))
		arr[i][j - 1] = false;
	if (isPointOnBoard(i -1, j))
		arr[i -1][j] = false;
}
//updated two dimientional isPointLegal[10[10],detemine which point (x,y) is valid=> isPointLegal[x][y] = true, else isPointLegal[x][y] = false
void NaiveAlgo::initIsPointLegal(const vector<Ship*>& shipList)
{
	memset(isPointLegal, true, sizeof(isPointLegal[0][0]) * BOARD_SIZE * BOARD_SIZE);   //all initialized to true
	for(int i = 0 ; i < shipList.size(); i++)
	{
		for (int j = 0 ; j < shipList[i]->size ; j++)
		{
			changeEnvalopPointsToFalse( isPointLegal , shipList[i]->pointList[j].x , shipList[i]->pointList[j].y);
		}
	}
}

void NaiveAlgo::initPotentialMoves(const vector<Ship*>& shipList)
{
	int index = 0;
	initIsPointLegal(shipList);
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
