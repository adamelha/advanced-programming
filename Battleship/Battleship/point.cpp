#include "point.h"
Point& Point::operator= (const Coordinate&rhs)
{
	this->row = rhs.row;
	this->col = rhs.col;
	this->depth = rhs.depth;

	return *this;
}