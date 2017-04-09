#pragma once
#include <stdint.h>

/**** Point Class *****/
class Point {
public:
	size_t x;
	size_t y;
	Point(size_t x, size_t y) : x(x), y(y) {}
	Point() {};
};

inline bool operator==(const Point& lhs, const Point& rhs) { return (lhs.x == rhs.x && lhs.y == rhs.y); }