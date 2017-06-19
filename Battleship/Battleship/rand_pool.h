#pragma once
#include "point.h"
#include "macros.h"
#include <random>
#include <ctime> 

class RandPool {
public:
	RandPool(std::vector < std::vector< std::vector<char> > > & _myBoard, int rows, int cols, int depth);
	~RandPool();
	Point getNextRandPoint();
	void removePointFromPool(Point pointToRemove);
private:
	//char ***myBoard;
	std::vector < std::vector< std::vector<char> > > & myBoard;

	// 1-dimension array containing potential moves
	Point *potentialMoves;
	int potentialMovesSize, rows, cols, depth;
	std::vector < std::vector< std::vector<Point *> > > refTabel;
	//Point ****refTabel;
	std::vector < std::vector< std::vector<bool> > > isPointLegal;
	//bool ***isPointLegal;
	void initIsPointLegal();
	void changeEnvalopPointsToFalse(std::vector < std::vector< std::vector<bool> > > &arr, Point p);
	bool isPointPartOfShip(Point p);
#if RANDOM_TRUE == 0

	std::mt19937 engine{ 0 };
#else
	std::mt19937 engine{ time(NULL) };
#endif
	std::uniform_int_distribution<> dist;
};

