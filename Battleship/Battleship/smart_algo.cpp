#include "smart_algo.h"
#include "macros.h"
#include "ships.h"
#include <cmath>
#include <algorithm>
#include "battle_utils.h"



SmartAlgo::SmartAlgo() : randPool(nullptr), randPoolTemp(), currentAttackIndex(0), directionsIndex(0), dirState2Points(0)
{
	
	
	// Init board to be array of char pointers, each pointing to a row in _board matrix (avoids dynamic allocation)
	// Now done in setBoard()
	
	// init directions
	directions[0] = UP_POINT;
	
	directions[1] = DOWN_POINT;
	directions[2] = RIGHT_POINT;
	directions[3] = LEFT_POINT;
	directions[4] = IN_POINT;
	directions[5] = OUT_POINT;
}

SmartAlgo::~SmartAlgo()
{
	delete randPool;
	// TODO: must free this->board
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

void SmartAlgo::setPlayer(int player)
{
	this->player = player;
}

void SmartAlgo::setBoard(const BoardData& board)
{
	squareType_e sqaretTypeIs;
	
	// Allocate local board
	//this->board = alloc3dArray<char>(board.rows(), board.cols(), board.depth());

	if (player)	//set Board to Player B
	{
		sqaretTypeIs = B_SQUARE;
	}
	else //set Board to Player A
	{
		sqaretTypeIs = A_SQUARE;
	}

	rows = board.rows();
	cols = board.cols();
	depth = board.depth();
	
	// Make allocations for 3D array
	setBoard(rows, cols, depth);
	
	this->player = player;
	for (int x = 0; x < board.rows(); x++)
	{
		for (int y = 0; y < board.cols(); y++)
		{
			for (int z = 0; z < board.depth(); z++)
			{
				if (isBelongToBoard(board.charAt(Coordinate(x + 1, y + 1, z + 1))) == sqaretTypeIs)
				{
					this->board[x][y][z] = board.charAt(Coordinate(x + 1, y + 1, z + 1));
					this->numOfSqares += 1;
					
				}
				else
					this->board[x][y][z] = ' ';       // initialize board to 
			}
		}
	}
	
	// Set RandPool data structure
	randPool = new RandPool(this->board, rows, cols, depth);
	
}

void SmartAlgo::setBoard(int rows, int cols, int depth)
{
	board = alloc3dArray<char>(rows, cols, depth);
}

Point SmartAlgo::getLowestPointIncurrentAttackArr()
{
	int minRow, minCol, minDepth;
	minRow = currentAttackArr[0].row;
	minCol = currentAttackArr[0].col;
	minDepth = currentAttackArr[0].depth;

	for (size_t i = 1; i < currentAttackIndex; i++)
	{
		
		minRow = min(minRow, currentAttackArr[i].row);
		minCol = min(minCol, currentAttackArr[i].col);
		minDepth = min(minDepth, currentAttackArr[i].depth);
	}

	return Point(minRow, minCol, minDepth);
}

Point SmartAlgo::getHighestPointIncurrentAttackArr()
{
	int maxRow, maxCol, maxDepth;
	maxRow = currentAttackArr[0].row;
	maxCol = currentAttackArr[0].col;
	maxDepth = currentAttackArr[0].depth;
	for (size_t i = 1; i < currentAttackIndex; i++)
	{
		maxRow = max(maxRow, currentAttackArr[i].row);
		maxCol = max(maxCol, currentAttackArr[i].col);
		maxDepth = max(maxDepth, currentAttackArr[i].depth);
	}

	return Point(maxRow, maxCol, maxDepth);
}


Point SmartAlgo::nextPointFrom2PointsDirPlus()
{
	// Add 1 to the max point
	Point dirPoint;
	Point maxPoint = getHighestPointIncurrentAttackArr();

	// Xs are different
	if (currentAttackArr[currentAttackIndex - 1].row != currentAttackArr[currentAttackIndex - 2].row)
	{
		return Point(maxPoint.row + 1, maxPoint.col, maxPoint.depth);
	}

	// Ys are different
	else if(currentAttackArr[currentAttackIndex - 1].col != currentAttackArr[currentAttackIndex - 2].col)
	{
		return Point(maxPoint.row, maxPoint.col + 1, maxPoint.depth);
	}

	// Zs are different
	else
	{
		return Point(maxPoint.row, maxPoint.col, maxPoint.depth + 1);
	}
}

Point SmartAlgo::nextPointFrom2PointsDirMinus()
{
	// Subtract 1 from min point
	Point dirPoint;
	Point minPoint = getLowestPointIncurrentAttackArr();

	// Xs are different
	if (currentAttackArr[currentAttackIndex - 1].row != currentAttackArr[currentAttackIndex - 2].row)
	{
		return Point(minPoint.row - 1, minPoint.col, minPoint.depth);
	}

	// Ys are different
	if (currentAttackArr[currentAttackIndex - 1].col != currentAttackArr[currentAttackIndex - 2].col)
	{
		return Point(minPoint.row, minPoint.col - 1, minPoint.depth);
	}

	// Zs are different
	else
	{
		return Point(minPoint.row, minPoint.col, minPoint.depth - 1);
	}
}


Coordinate SmartAlgo::attack()
{
	Point attackPoint;
	Point outOfMoves = Point(ALGO_OUT_OF_MOVES, ALGO_OUT_OF_MOVES, ALGO_OUT_OF_MOVES);
	
	// If not in middle of attack, attack at random
	if (currentAttackIndex == 0)
	{
		attackPoint = randPool->getNextRandPoint();
		/*DEBUG START*/
		if (attackPoint.row == 3 && attackPoint.col == 0 && attackPoint.depth == 3)
		{
			attackPoint.row = attackPoint.row;
		}
		/*DEBUG END*/
	}
	
	
	// If found first part of ship but not rest, guess near
	else if(currentAttackIndex == 1)
	{
		attackPoint = Point(currentAttackArr[0].row + directions[directionsIndex].row,
			currentAttackArr[0].col + directions[directionsIndex].col,
			currentAttackArr[0].depth + directions[directionsIndex].depth);
		
		// Make sure ipint on board if not iterate directions[] until it is
		while (!isPointOnBoard(attackPoint.row, attackPoint.col, attackPoint.depth))
		{
			directionsIndex++;
			attackPoint = Point(currentAttackArr[0].row + directions[directionsIndex].row,
				currentAttackArr[0].col + directions[directionsIndex].col,
				currentAttackArr[0].depth + directions[directionsIndex].depth);
		}

		// Should not happen
		if (directionsIndex < DIRECTIONS_FINAL_INDEX) {
			//directionsIndex++;
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
			if (!isPointOnBoard(attackPoint.row, attackPoint.col, attackPoint.depth))
			{
				dirState2Points++;
				attackPoint = nextPointFrom2PointsDirMinus();
			}
		}
		else
		{
			attackPoint = nextPointFrom2PointsDirMinus();
			if (!isPointOnBoard(attackPoint.row, attackPoint.col, attackPoint.depth))
			{
				dirState2Points++;
				attackPoint = nextPointFrom2PointsDirPlus();
			}
		}

		//Remove attack point from randPool
		randPool->removePointFromPool(attackPoint);
	}

	

	return Coordinate(attackPoint.row + 1, attackPoint.col + 1, attackPoint.depth + 1);
}

void SmartAlgo::notifyOnAttackResult(int player, Coordinate move, AttackResult result)
{
	int adjustedRow = move.row - 1;
	int adjustedCol = move.col -1;
	int adjustedDepth = move.depth - 1;

	// If opponent attacked
	if (player != this->player)
	{
		// Remove points attacked by opponent
		this->randPool->removePointFromPool(Point(adjustedRow, adjustedCol, adjustedDepth));
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
			currentAttackArr[currentAttackIndex] = Point(adjustedRow, adjustedCol, adjustedDepth);
			currentAttackIndex++;	
		}

		if (result == AttackResult::Miss)
		{
			if (currentAttackIndex > 1)
			{
				dirState2Points++;
			}
			else if(currentAttackIndex == 1)
			{
				directionsIndex++;
			}
		}
	}
}


ALGO_API IBattleshipGameAlgo* GetAlgorithm() {
	
	return new SmartAlgo();
}

