#pragma once
#include <stdint.h>
#include <vector>
#include <iostream>
#include <algorithm>


using namespace std;

/**** Point Class *****/
template <int DIMENSIONS>
class Point{
public:
	vector<int> coordinates;
	Point(const std::initializer_list<int>& values) {
		coordinates.resize(DIMENSIONS);
		int i = 0;
		for (auto& val : values) {
			coordinates[i++] = val;
		}
	}
	Point() : coordinates{} { coordinates.resize(DIMENSIONS); }

	Point& operator= (const Point<DIMENSIONS>&rhs) {
		coordinates = rhs.coordinates;
		return *this;
	}

	bool operator < (const Point<DIMENSIONS>& point) const
	{
		for (size_t i = 0; i < DIMENSIONS; i++)
		{
			if (coordinates[i] < point.coordinates[i])
			{
				return true;
			}
			if (coordinates[i] > point.coordinates[i])
			{
				return false;
			}
		}

		return false;
	}

	typedef vector<int>::const_iterator const_iterator;
	const_iterator begin() const { return coordinates.begin(); }
	const_iterator end() const { return coordinates.end(); }

};


template <int DIMENSIONS>
inline Point<DIMENSIONS> operator+(const Point<DIMENSIONS>& lhs, const Point<DIMENSIONS>& rhs) {
	Point<DIMENSIONS> retPoint;
	for (size_t i = 0; i < DIMENSIONS; i++)
	{
		retPoint.coordinates[i] = lhs.coordinates[i] + rhs.coordinates[i];
	}

	return retPoint;
}

// For debug use!!
template <int DIMENSIONS>
std::ostream &operator<<(std::ostream &os, Point<DIMENSIONS> const &p) {
	os << "<";
	for (size_t i = 0; i < DIMENSIONS; i++)
	{
		os << " " << p.coordinates[i];
	}
	os << ">";

	return os;
}


