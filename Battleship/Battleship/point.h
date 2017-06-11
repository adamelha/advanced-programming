#pragma once
#include <stdint.h>
#include <utility> // for std::pair
#include "IBattleshipGameAlgo.h"

#define INVALID_POINT	-1

using namespace std;

/**** Point Class *****/
class Point : public Coordinate{
public:
	Point(int row, int col, int depth) : Coordinate(row, col, depth) {};
	Point() : Coordinate(0,0,0) {};
	void invalidatePoint() { row = INVALID_POINT; col = INVALID_POINT; depth = INVALID_POINT; }
	bool isPointValid() { return (row == INVALID_POINT) && (col == INVALID_POINT) && (depth == INVALID_POINT); }
	std::pair<int, int> toPair() { return std::pair<int, int>(row, col); }
	Point& operator= (const Coordinate&rhs);
};

inline bool operator==(const Point& lhs, const Point& rhs) { return (lhs.row == rhs.row && lhs.col == rhs.col && lhs.depth == rhs.depth); }
inline bool operator!=(const Point& lhs, const Point& rhs) { return !(lhs == rhs); }
