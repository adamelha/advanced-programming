#include <iostream>
#include <algorithm>
#include <list>
#include <map>
#include <functional>
#include <string>
#include <cstring>
#include "matrix.h"

//using namespace std;
template <typename T>
using deref_iter_t = std::remove_reference_t<decltype(*std::declval<T>())>;
template<class Iterator, class GroupingFunc>
auto groupValues(Iterator begin, Iterator end, GroupingFunc groupingFunc) {
	using T = deref_iter_t<Iterator>;
	using GroupingType = std::result_of_t<GroupingFunc(T&)>;
	std::map<GroupingType, std::list<T>> groups;



	std::for_each(begin, end, [&groups, groupingFunc](const auto& val) {
		groups[groupingFunc(val)].push_back(val);
	});
	return groups;
}
template<class MapOfLists>
void print(const MapOfLists& m) {
	for (const auto& p : m) {
		std::cout << p.first << std::endl << "-----" << std::endl;
		for (const auto& val : p.second) {
			std::cout << val << std::endl;
		}
		std::cout << "-----" << std::endl;
	}
}
int main() {
	/*std::list<std::string> strs = { "hello", "world", "Hello", "World" };
	auto groupOfStrings = groupValues(strs.begin(), strs.end(), [](auto& val) {
		return (char)std::toupper(val.at(0));
	});
	print(groupOfStrings);
	*/

	//Point<2> p = { 0,0 };
	char c;
	//Matrix2d<char> m = { { 'a', 'A', 'a' },{ 'B', 'a', 'B' },{ 'B', 'a', 'B' } };
	Matrix2d<char> m = { { 'a', 'b', 'c' ,'1' },{ 'd', 'e', 'f', '2' },{ 'g', 'h', 'i', '3' } };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Point<2> p = { i, j };
			c = m[p];

			cout << c << "\n";
			
		}

	}

	

	//char c = m[p];

	//MatrixPrinter<int, 2>::print()
	cout << m;

	std::list<int> numbers = { 1, 5, 10, 24, 13 };
	auto groupOfNumbers = groupValues(numbers.begin(), numbers.end(), [](int val) {
		int decile = int(val / 10) * 10;
		return std::to_string(decile) + '-' + std::to_string(decile + 9);
	});
	print(groupOfNumbers);
}


/*
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

	//cout << m0[0] << endl;


	Matrix<int, 3> m1 = { { { 10, 20, 30 },{ 10, 20 },{ 11, 22, 33 } },{ { 40, 50, 60 },{ 80, 90 },{ 51, 52, 53 } } };
	for (int i = 0 ; i < m1.size() ; i++)
		cout << m1[i] << endl;



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
	Matrix2d<char> m = { { 'a', 'A', 'a' },{ 'B', 'a', 'B' },{ 'B', 'a', 'B' } };
	//m.groupValues([](auto i) {return islower(i) ? "L" : "U"; });

	//auto all_groups = m.groupValues([](auto i) {return islower(i) ? "L" : "U"; });
	//print(all_groups);

	mainFromLecture();
}
*/