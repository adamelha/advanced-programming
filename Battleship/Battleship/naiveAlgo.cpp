#include "naiveAlgo.h"
#define isPointOnBoard(i , j)  ( i < arrSize && i > -1 && j < arrSize && j > -1 )

Point naiveAlgo::getNextPotentialPoint()
{
	return Point();
}

Point * naiveAlgo::potentialLegalMoves()
{
	return nullptr;
}

int naiveAlgo::getPotentialMovesSize()
{
	return potentialMovesSize;
}

void naiveAlgo::setBoard(const char ** board, int numRows, int numCols)
{
}

std::pair<int, int> naiveAlgo::attack()
{
	int curr = this->nextPointIndex;
	this->nextPointIndex += 1;
	if (this->nextPointIndex < getPotentialMovesSize() )
		return std::pair<int, int>(potentialMoves[curr].x , potentialMoves[curr].y);
	return std::pair<int, int>(-1,-1);
}

void naiveAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
}


//changes unvalid points near a point with player ship
void changeEnvalopPointsToFalse(bool arr[][arrSize] , int i ,int j)
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
void naiveAlgo::initIsPointLegal(const vector<Ship*>& shipList)
{
	memset(isPointLegal, true, sizeof(isPointLegal[0][0]) * arrSize * arrSize);   //all initialized to true
	for(int i = 0 ; i < shipList.size(); i++)
	{
		for (int j = 0 ; j < shipList[i]->size ; j++)
		{
			changeEnvalopPointsToFalse( isPointLegal , shipList[i]->pointList[j].x , shipList[i]->pointList[j].y);
		}
	}
}

void naiveAlgo::initPotentialMoves(const vector<Ship*>& shipList)
{
	int index = 0;
	naiveAlgo::initIsPointLegal(shipList);
	for (int i = 0 ; i < arrSize; i++)
	{
		for (int j =0 ; j < arrSize ; j++)
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
