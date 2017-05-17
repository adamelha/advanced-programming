#pragma once
#include "point.h"
#include "macros.h"

class RandPool {
public:
	RandPool(char **myBoard);
	~RandPool();
	Point getNextRandPoint();
	void removePointFromPool(Point pointToRemove);
private:
	char **myBoard;
	Point potentialMoves[BOARD_SIZE * BOARD_SIZE];
	int potentialMovesSize;
	Point *refTabel[BOARD_SIZE][BOARD_SIZE];
	bool isPointLegal[BOARD_SIZE][BOARD_SIZE];
	void initIsPointLegal();
	void changeEnvalopPointsToFalse(bool arr[][BOARD_SIZE], int x, int y);
	bool isPointPartOfShip(int x, int y);
};

