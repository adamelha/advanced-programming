#include "rand_pool.h"
#include "point.h"
#include <cstdlib>

#define isPointOnBoard(i , j)  ( i < BOARD_SIZE && i > -1 && j < BOARD_SIZE && j > -1 )

// Cntr Dtr
RandPool::RandPool(char **myBoard) : potentialMoves(), myBoard(myBoard), refTabel()
{
	// Init potential moves
	int index = 0;
	initIsPointLegal();
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (isPointLegal[i][j])
			{
				potentialMoves[index] = Point(i, j);
				index += 1;
				potentialMovesSize += 1;
			}

		}
	}

	//init ref tabel
	for (size_t i = 0; i < potentialMovesSize; i++)
	{
		refTabel[potentialMoves[i].x][potentialMoves[i].y] = &potentialMoves[i];

	}
}
/*RandPool::~RandPool()
{
	delete[] potentialMoves;
}*/

// Public methods

// Retrieves next random point from potentialMoves, removes it, and removes its refrence from refTabel
// Use when you are not in middle of attack and wish to hit new random ship 
// This removes point from the potential moves
Point RandPool::getNextRandPoint()
{
	int r;
	Point retPoint;
	
	// If no potential moves return out of moves
	if (potentialMovesSize == 0)
	{
		return Point(ALGO_OUT_OF_MOVES, ALGO_OUT_OF_MOVES);
	}

	// Gen random point from potentialMovesSize
	r = rand() % potentialMovesSize;
	retPoint = potentialMoves[r];

	//replace the removed point with the last one and update the size 
	potentialMoves[r] = potentialMoves[potentialMovesSize - 1];
	potentialMovesSize--;

	//update the refrence to point in the new location
	refTabel[potentialMoves[r].x][potentialMoves[r].y] = &potentialMoves[r];

	//Nullify retPoint in refTabel
	refTabel[retPoint.x][retPoint.y] = nullptr;

	return retPoint;
}

// Use after attack a specific point, not at random (so you would never attack this point again)
// Or when you know that you will never have to attack this point
void RandPool::removePointFromPool(Point pointToRemove)
{
	Point *pPointToRemoveInPotentialMoves;

	// If not a potential move, do nothing
	if (refTabel[pointToRemove.x][pointToRemove.y] == nullptr)
	{
		return;
	}
	pPointToRemoveInPotentialMoves = refTabel[pointToRemove.x][pointToRemove.y];

	// Remove the point by replacing it with the last point in potentialMoves and update size
	*pPointToRemoveInPotentialMoves = potentialMoves[potentialMovesSize - 1];
	potentialMovesSize--;
}

bool RandPool::isPointPartOfShip(int x, int y)
{
	return	myBoard[x][y] == 'b'
		|| myBoard[x][y] == 'B'
		|| myBoard[x][y] == 'p'
		|| myBoard[x][y] == 'P'
		|| myBoard[x][y] == 'm'
		|| myBoard[x][y] == 'M'
		|| myBoard[x][y] == 'd'
		|| myBoard[x][y] == 'D';
}

//changes unvalid points near a point with player ship
void RandPool::changeEnvalopPointsToFalse(bool arr[][BOARD_SIZE], int x, int y)
{
	if (isPointPartOfShip(x, y))
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


void RandPool::initIsPointLegal()
{
	memset(isPointLegal, true, sizeof(isPointLegal[0][0]) * BOARD_SIZE * BOARD_SIZE);   //all initialized to true

	for (int x = 0; x < BOARD_SIZE; x++)
	{
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			changeEnvalopPointsToFalse(isPointLegal, x, y);
		}
	}
}