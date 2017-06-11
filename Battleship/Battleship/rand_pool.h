#pragma once
#include "point.h"
#include "macros.h"

class RandPool {
public:
	RandPool(char ***myBoard, int rows, int cols, int depth);
	~RandPool();
	Point getNextRandPoint();
	void removePointFromPool(Point pointToRemove);
private:
	char ***myBoard;

	// 1-dimension array containing potential moves
	Point *potentialMoves;
	int potentialMovesSize, rows, cols, depth;
	Point ****refTabel;
	bool ***isPointLegal;
	void initIsPointLegal();
	void changeEnvalopPointsToFalse(bool ***arr, Point p);
	bool isPointPartOfShip(Point p);

};

