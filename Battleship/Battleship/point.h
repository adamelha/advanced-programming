#pragma once
#include <stdint.h>

#define INVALID_POINT	-1
/**** Point Class *****/
class Point {
public:
	int x;
	int y;
	Point(int x, int y) : x(x), y(y) {}
	Point() {};
	void invalidatePoint() { x = INVALID_POINT; y = INVALID_POINT; }
	bool isPointValid() { return (x == INVALID_POINT) && (y == INVALID_POINT); }
};

inline bool operator==(const Point& lhs, const Point& rhs) { return (lhs.x == rhs.x && lhs.y == rhs.y); }