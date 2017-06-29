#pragma once
#include <stdint.h>
#include <vector>
#define INVALID_POINT	-1

using namespace std;

/**** Point Class *****/
template <int DIMENSIONS>
class Point{
public:
	int dimensions = DIMENSIONS;
	vector<int> coordinates;
	Point(int coordinates[DIMENSIONS]) {};
	Point(const std::initializer_list<int>& values) {
		coordinates.resize(DIMENSIONS);
		int i = 0;
		for (auto& val : values) {
			coordinates[i++] = val;
		}
	}
	Point() : coordinates{} { coordinates.resize(DIMENSIONS); }

	void invalidatePoint() { for (int i = 0; i < DIMENSIONS; i++) {coordinates[i] = INVALID_POINT}; }
	//bool isPointValid() { return (row == INVALID_POINT) && (col == INVALID_POINT) && (depth == INVALID_POINT); }
	Point& operator= (const Point<DIMENSIONS>&rhs) {
		for (int i = 0; i < DIMENSIONS; i++) {
			rhs.coordinates[i] = coordinates[i];
		}
	}
	// Don't copy the 0 index
	Point& operator= (const Point<DIMENSIONS + 1>&rhs) { 
		for (int i = 0; i < DIMENSIONS; i++) { 
			coordinates[i] = rhs.coordinates[i + 1];
		}
	}
};

template <int DIMENSIONS>
inline bool operator==(const Point<DIMENSIONS>& lhs, const Point<DIMENSIONS>& rhs) {
	for (int i = 0; i < DIMENSIONS; i++) {
		if (lhs.coordinates[i] != rhs.coordinates[i])
			return false;  
	} 
	return true;
}

template <int DIMENSIONS>
inline bool operator!=(const Point<DIMENSIONS>& lhs, const Point<DIMENSIONS>& rhs) { return !(lhs == rhs); }
