#include "rand_pool.h"
#include "point.h"
#include <cstdlib>
#include "battle_utils.h"
#include "macros.h"
#include "battle_utils.h"
#include <ctime> 

// Cntr Dtr
RandPool::RandPool(char ***myBoard, int _rows, int _cols, int _depth) : potentialMoves(), myBoard(myBoard), refTabel(), rows(_rows), cols(_cols), depth(_depth), potentialMovesSize(0)
{
	// Init potential moves
	int index = 0;
	
	initIsPointLegal();
	
	// Allocate potential moves array to max size
	potentialMoves = new Point[rows * cols * depth]();
	
	// Init potentialMoves list 
	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < cols; y++)
		{
			for (int z = 0; z < depth; z++)
			{
				if (isPointLegal[x][y][z])
				{
					potentialMoves[index] = Point(x, y, z);
					index += 1;
					potentialMovesSize += 1;
				}
			}
			

		}
	}

	// Allocate ref tabel
	refTabel = alloc3dArray<Point *>(rows, cols, depth);
	
	// Nullify ref tabel
	init3dArray<Point *>(refTabel, nullptr, rows, cols, depth);
	//init ref tabel

	for (size_t i = 0; i < potentialMovesSize; i++)
	{
		DEBUG_PRINT("refTabel[%d][%d][%d], i = %d, potentialMovesSize = %d\n", potentialMoves[i].row, potentialMoves[i].col, potentialMoves[i].depth, i, potentialMovesSize);
		refTabel[potentialMoves[i].row][potentialMoves[i].col][potentialMoves[i].depth] = &potentialMoves[i];
	}
	

	/* if in release mode - initialize random seed.
	*  if DEBUG is defined - use seed 0 so random sequence remains the same during debug 
	*/
#if RANDOM_TRUE == 0
	srand(0);
#else
	srand(time(NULL));
#endif // DEBUG
	
	
}
RandPool::~RandPool()
{
	delete[] potentialMoves;
	// Must delete refTabel
}

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
		return Point(ALGO_OUT_OF_MOVES, ALGO_OUT_OF_MOVES, ALGO_OUT_OF_MOVES);
	}

	// Gen random point from potentialMovesSize
	r = rand() % potentialMovesSize;
	retPoint = potentialMoves[r];

	//replace the removed point with the last one and update the size 
	potentialMoves[r] = potentialMoves[potentialMovesSize - 1];
	potentialMovesSize--;

	//update the refrence to point in the new location
	refTabel[potentialMoves[r].row][potentialMoves[r].col][potentialMoves[r].depth] = &potentialMoves[r];

	//Nullify retPoint in refTabel
	refTabel[retPoint.row][retPoint.col][retPoint.depth] = nullptr;

	return retPoint;
}

// Use after attack a specific point, not at random (so you would never attack this point again)
// Or when you know that you will never have to attack this point
void RandPool::removePointFromPool(Point pointToRemove)
{
	Point *pPointToRemoveInPotentialMoves;

	// If not a potential move, do nothing
	if (refTabel[pointToRemove.row][pointToRemove.col][pointToRemove.depth] == nullptr)
	{
		return;
	}
	pPointToRemoveInPotentialMoves = refTabel[pointToRemove.row][pointToRemove.col][pointToRemove.depth];

	
	// Remove the point by replacing it with the last point in potentialMoves and update size
	if (potentialMovesSize > 0) {
		*pPointToRemoveInPotentialMoves = potentialMoves[potentialMovesSize - 1];
		potentialMovesSize--;
	}
}

bool RandPool::isPointPartOfShip(Point p)
{
	return	myBoard[p.row][p.col][p.depth] == 'b'
		|| myBoard[p.row][p.col][p.depth] == 'B'
		|| myBoard[p.row][p.col][p.depth] == 'p'
		|| myBoard[p.row][p.col][p.depth] == 'P'
		|| myBoard[p.row][p.col][p.depth] == 'm'
		|| myBoard[p.row][p.col][p.depth] == 'M'
		|| myBoard[p.row][p.col][p.depth] == 'd'
		|| myBoard[p.row][p.col][p.depth] == 'D';
}

//changes unvalid points near a point with player ship
void RandPool::changeEnvalopPointsToFalse(bool ***arr, Point p)
{
	if (isPointPartOfShip(p))
	{
		arr[p.row][p.col][p.depth] = false;
		if (isPointOnBoard(p.row + 1, p.col, p.depth))
			arr[p.row + 1][p.col][p.depth] = false;
		if (isPointOnBoard(p.row, p.col + 1, p.depth))
			arr[p.row][p.col + 1][p.depth] = false;
		if (isPointOnBoard(p.row, p.col - 1., p.depth))
			arr[p.row][p.col - 1][p.depth] = false;
		if (isPointOnBoard(p.row - 1, p.col, p.depth))
			arr[p.row - 1][p.col][p.depth] = false;
		if (isPointOnBoard(p.row, p.col, p.depth + 1))
			arr[p.row][p.col][p.depth + 1] = false;
		if (isPointOnBoard(p.row, p.col, p.depth - 1))
			arr[p.row][p.col][p.depth - 1] = false;

	}
}


void RandPool::initIsPointLegal()
{
	// Allocate isPointLegal
	isPointLegal = alloc3dArray<bool>(rows, cols, depth);
	
	// Initialize all to true
	init3dArray<bool>(isPointLegal, true, rows, cols, depth);

	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < cols; y++)
		{
			for (int z = 0; z < depth; z++)
			{
				changeEnvalopPointsToFalse(isPointLegal, Point(x, y, z));
			}
			
		}
	}
}