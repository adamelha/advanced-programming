#include <iostream>
#include <algorithm>
#include <list>
#include <map>
#include <functional>
#include <string>
#include <cstring>
#include "matrix.h"

using namespace std;

template<typename Groups>
void print(const Groups& all_groups) {
	for (const auto& groupType : all_groups) {
		cout << groupType.first << ":" << endl;
		for (const auto& groupOfType : groupType.second) {
			for (const auto& coord : groupOfType) {
				cout << "{ ";
				for (auto i : coord) {   // was: for(int i : coord)
					cout << i << ' ';
				}
				cout << "} ";
			}
			cout << endl;
		}
	}
}

int main() {
	
	// Q1
	Matrix2d<char> m = { { 'a', 'A', 'a' },{ 'B', 'a', 'B' },{ 'B', 'a', 'B' } };
	auto all_groups = m.groupValues([](auto i) {return islower(i) ? "L" : "U"; });
	print(all_groups);
	
	// Q2
	Matrix3d<int> m2 = { { { 1, 2, 3 },{ 1, 2 },{ 1, 2 } },{ { 1, 2 },{ 1, 2, 3, 4 } } };
	auto groups = m2.groupValues([](auto i) {return i % 3 ? "!x3" : "x3"; });
	print(groups);

	// Bonus
	Matrix<int, 4> m4 = { { { {1,2},{3,4} },{ {5,6},{7,8} } }, { { {9,10},{11,12} },{ {13,14},{15,16} } } };
	auto groups4 = m2.groupValues([](auto i) {return i % 4 ? "!x4" : "x4"; });
	print(groups4);
}


