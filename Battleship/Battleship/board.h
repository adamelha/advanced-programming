#pragma once

#include <string>
#include "error_class.h"
#define BOARD_SIZE						10
#define NUM_OF_BOARD_ERR_MSGS		8

#define BOARD_DIMENSIONS_INVALID		"Board dimensions are invalid\n"
#define BOARD_DIMENSIONS_INVALID_IDX	0

using namespace std;

/**** Point Class *****/
class Point {
public:
	uint32_t x, y;
	Point(uint32_t x, uint32_t y) : x(x), y(y) {}
};


/**** Board Class *****/

enum Board_t { BOARD_OK, BOARD_ERROR, BOARD_FILE_NOT_VALID };

class Board : public ErrorClass {
public:
	Board_t parse();
	string getFilesPath();
	char getCharFromBoard(Point p);
	Board(string boardStringFromFile) : ErrorClass(NUM_OF_BOARD_ERR_MSGS), board(), boardStringFromFile(boardStringFromFile) {}

private:
	char board[BOARD_SIZE][BOARD_SIZE];
	string boardStringFromFile;
	void setCharOnBoard(Point p, char val);
	std::istream& Board::safeGetline(std::istream& is, std::string& t);
};
