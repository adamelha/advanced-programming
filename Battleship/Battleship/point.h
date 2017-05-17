#pragma once
#include <stdint.h>
#include <utility> // for std::pair

#define INVALID_POINT	-1

using namespace std;

/**** Point Class *****/
class Point {
public:
	int x;
	int y;
	Point(int x, int y) : x(x), y(y) {}
	Point() : x(0), y(0) {};
	void invalidatePoint() { x = INVALID_POINT; y = INVALID_POINT; }
	bool isPointValid() { return (x == INVALID_POINT) && (y == INVALID_POINT); }
	std::pair<int, int> toPair() { return std::pair<int, int>(x, y); }
};

inline bool operator==(const Point& lhs, const Point& rhs) { return (lhs.x == rhs.x && lhs.y == rhs.y); }
inline bool operator!=(const Point& lhs, const Point& rhs) { return !(lhs == rhs); }