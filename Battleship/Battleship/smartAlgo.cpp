#include "smartAlgo.h"
#define isPointOnBoard(i , j)  ( i < arrSize && i > -1 && j < arrSize && j > -1 )
enum { up = 0, right = 1, buttom = 2, left = 3 };

Point smartAlgo::getNextPotentialPoint()
{
	return Point();
}

Point * smartAlgo::potentialLegalMoves()
{
	return nullptr;
}

int smartAlgo::getPotentialMovesSize()
{
	return potentialMovesSize;
}

void smartAlgo::setBoard(const char ** board, int numRows, int numCols)
{
}

std::pair<int, int> smartAlgo::attack()
{
	int r;
	Point attack;
	srand ( time(NULL) );
	if (this->strike)       // playr indeed Hit! (in prev turn)
	{
		//TBD: local enviorment: return point from local env,the point who got hit is 
		this;
	} 
	else
	{                     
		r = rand() % potentialMovesSize;
		attack = this->getNextPotentialPoint[r];
		this->getNextPotentialPoint[r] = this->potentialMoves[potentialMovesSize];
		PointerForPotentialMove[this->potentialMoves[r].x][this->potentialMoves[r].y] = &this->potentialMoves[r];
		potentialMovesSize -= 1;

		return std::pair<int, int>(attack.x, attack.y);
	}

	return std::pair<int, int>(-1 , -1);
}

void smartAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
}


//changes unvalid points near a point with player ship
void changeEnvalopPointsToFalse(bool arr[][arrSize], int i, int j)
{
	arr[i][j] = false;
	if (isPointOnBoard(i + 1, j))
		arr[i + 1][j] = false;
	if (isPointOnBoard(i, j + 1))
		arr[i][j + 1] = false;
	if (isPointOnBoard(i, j - 1))
		arr[i][j - 1] = false;
	if (isPointOnBoard(i - 1, j))
		arr[i - 1][j] = false;
}
//updated two dimientional isPointLegal[10[10],detemine which point (x,y) is valid=> isPointLegal[x][y] = true, else isPointLegal[x][y] = false
void smartAlgo::initIsPointLegal(const vector<Ship*>& shipList)
{
	memset(isPointLegal, true, sizeof(isPointLegal[0][0]) * arrSize * arrSize);   //all initialized to true
	for (int i = 0; i < shipList.size(); i++)
	{
		for (int j = 0; j < shipList[i]->size; j++)
		{
			changeEnvalopPointsToFalse(isPointLegal, shipList[i]->pointList[j].x, shipList[i]->pointList[j].y);
		}
	}
}

void smartAlgo::initPotentialMoves(const vector<Ship*>& shipList)
{
	int index = 0;
	smartAlgo::initIsPointLegal(shipList);
	for (int i = 0; i < arrSize; i++)
	{
		for (int j = 0; j < arrSize; j++)
		{
			if (isPointLegal[i][j])
			{
				potentialMoves[index] = Point(i, j);
				index += 1;
				potentialMovesSize += 1;
			}

		}
	}

	//memset(array, 0, sizeof(array[0][0]) * m * n);

}