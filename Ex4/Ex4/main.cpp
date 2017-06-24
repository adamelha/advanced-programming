#include "matrix.h"

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

int mainFromLecture()
{
	Matrix<int, 1> m0 = { 25, 10, 15 };

	cout << m0 << endl;


	Matrix<int, 3> m1 = { { { 10, 20, 30 },{ 10, 20 },{ 11, 22, 33 } },{ { 40, 50, 60 },{ 80, 90 },{ 51, 52, 53 } } };

	cout << m1 << endl;



	// move-assignment

	Matrix3d<int> m2;

	m2 = std::move(m1);

	cout << m2 << endl;


	// move-ctor

	auto m3 = std::move(m2);

	cout << m3 << endl;



	IntVector m4 = std::move(m0);

	cout << m4 << endl;


	Matrix<int, 2> m5 = { {},{ 25, 10, 15 },{},{ 3 } };

	cout << m5 << endl;

	return 0;
}

int main() {

	// Main that we must support - Matrix2d<char> m works.
	// Must do the rest
	//Matrix2d<char> m = { { 'a', 'A', 'a' },{ 'B', 'a', 'B' },{ 'B', 'a', 'B' } };
	//auto all_groups = m.groupValues([](auto i) {return islower(i) ? "L" : "U"; });
	//print(all_groups);

	mainFromLecture();
}
