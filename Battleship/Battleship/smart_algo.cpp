#include "smart_algo.h"
#include "macros.h"
#include "ships.h"
#include <cmath>

#define isPointOnBoard(i , j)  ( i < BOARD_SIZE && i > -1 && j < BOARD_SIZE && j > -1 )

#define UP_POINT	(Point(0, 1))
#define DOWN_POINT	(Point(0, -1))
#define RIGHT_POINT	(Point(1, 0))
#define LEFT_POINT	(Point(-1, 0))



SmartAlgo::SmartAlgo() : randPool(nullptr), randPoolTemp(), currentAttackIndex(0), directionsIndex(0), dirState2Points(0)
{
	// Init board to be array of char pointers, each pointing to a row in _board matrix (avoids dynamic allocation)

	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		board[i] = &(_board[i][0]);
	}

	// init directions
	directions[0] = UP_POINT;
	directions[1] = DOWN_POINT;
	directions[2] = RIGHT_POINT;
	directions[3] = LEFT_POINT;

}

SmartAlgo::~SmartAlgo()
{
	delete randPool;
}


// Gal, here you must call the initializer! I don't think you can use ship list as an argument.
// Must create it here!
bool SmartAlgo::init(const std::string& path)
{
	randPool = new RandPool(board);
	return true;
}

Point SmartAlgo::getNextPotentialPoint()
{
	return Point();
}

Point * SmartAlgo::potentialLegalMoves()
{
	return nullptr;
}

int SmartAlgo::getPotentialMovesSize()
{
	return potentialMovesSize;
}

void SmartAlgo::setBoard(int player, const char ** board, int numRows, int numCols)
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

	this->player = player;
	for (int x = 0; x < numRows; x++)
	{
		for (int y = 0; y < numCols; y++)
		{
			if (isBelongToBoard(board[x][y]) == sqaretTypeIs)
			{
				this->board[x][y] = board[x][y];
				this->numOfSqares += 1;
			}
			else
				this->board[x][y] = ' ';       // initialize board to 
		}
	}


}

Point SmartAlgo::getLowestPointIncurrentAttackArr()
{
	int minx, miny;
	minx = currentAttackArr[0].x;
	miny = currentAttackArr[0].y;

	for (size_t i = 1; i < currentAttackIndex; i++)
	{
		minx = min(minx, currentAttackArr[i].x);
		miny = min(miny, currentAttackArr[i].y);
	}

	return Point(minx, miny);
}

Point SmartAlgo::getHighestPointIncurrentAttackArr()
{
	int maxx, maxy;
	maxx = currentAttackArr[0].x;
	maxy = currentAttackArr[0].y;

	for (size_t i = 1; i < currentAttackIndex; i++)
	{
		maxx = max(maxx, currentAttackArr[i].x);
		maxy = max(maxy, currentAttackArr[i].y);
	}

	return Point(maxx, maxy);
}


Point SmartAlgo::nextPointFrom2PointsDirPlus()
{
	// Add 1 to the max point
	Point dirPoint;
	Point maxPoint = getHighestPointIncurrentAttackArr();

	// Xs are different
	if (currentAttackArr[currentAttackIndex - 1].x != currentAttackArr[currentAttackIndex - 2].x)
	{
		return Point(maxPoint.x + 1, maxPoint.y);
	}

	// Ys are different
	else
	{
		return Point(maxPoint.x, maxPoint.y + 1);
	}
	
}

Point SmartAlgo::nextPointFrom2PointsDirMinus()
{
	// Subtract 1 from min point
	Point dirPoint;
	Point minPoint = getLowestPointIncurrentAttackArr();

	// Xs are different
	if (currentAttackArr[currentAttackIndex - 1].x != currentAttackArr[currentAttackIndex - 2].x)
	{
		return Point(minPoint.x - 1, minPoint.y);
	}

	// Ys are different
	else
	{
		return Point(minPoint.x, minPoint.y - 1);
	}
}


std::pair<int, int> SmartAlgo::attack()
{
	Point attackPoint;
	Point outOfMoves = Point(ALGO_OUT_OF_MOVES, ALGO_OUT_OF_MOVES);
	
	// If not in middle of attack, attack at random
	if (currentAttackIndex == 0)
	{
		attackPoint = randPool->getNextRandPoint();

	}
	
	// If found first part of ship but not rest, guess near
	else if(currentAttackIndex == 1)
	{
		attackPoint = Point(currentAttackArr[0].x + directions[directionsIndex].x,
			currentAttackArr[0].y + directions[directionsIndex].y);
		
		// Make sure ipint on board if not iterate directions[] until it is
		while (!isPointOnBoard(attackPoint.x, attackPoint.y))
		{
			directionsIndex++;
			attackPoint = Point(currentAttackArr[0].x + directions[directionsIndex].x,
				currentAttackArr[0].y + directions[directionsIndex].y);
		}

		// Should not happen
		if (directionsIndex < 3) {
			directionsIndex++;
		}

		//Remove attack point from randPool
		randPool->removePointFromPool(attackPoint);
	}

	// already know direction, currentAttackIndex > 1
	else
	{
		// Get direction based on the 2 last points found
		if (dirState2Points == 0)
		{
			attackPoint = nextPointFrom2PointsDirPlus();
			if (!isPointOnBoard(attackPoint.x, attackPoint.y))
			{
				dirState2Points++;
				attackPoint = nextPointFrom2PointsDirMinus();
			}
		}
		else
		{
			attackPoint = nextPointFrom2PointsDirMinus();
		}

		//Remove attack point from randPool
		randPool->removePointFromPool(attackPoint);
	}

	

	return std::pair<int, int>(attackPoint.x + 1, attackPoint.y + 1);
}

void SmartAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	int adjustedRow = row - 1;
	int adjustedCol = col -1;

	// If opponent attacked
	if (player != this->player)
	{
		// Remove points attacked by opponent
		this->randPool->removePointFromPool(Point(adjustedRow, adjustedCol));
	}

	// If I am the one who attacked
	else
	{
		if (result == AttackResult::Sink)
		{
			// Restore state
			currentAttackIndex = 0;
			directionsIndex = 0;
			dirState2Points = 0;
		}

		if (result == AttackResult::Hit)
		{	
			currentAttackArr[currentAttackIndex] = Point(adjustedRow, adjustedCol);
			currentAttackIndex++;	
		}

		if (result == AttackResult::Miss)
		{
			if (currentAttackIndex > 1)
			{
				dirState2Points++;
			}
		}
	}
}


