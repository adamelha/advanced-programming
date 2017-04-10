#pragma once

#include <string>
#include "error_class.h"
#include "status.h"
#include "point.h"
#include "ships.h"
#include <ctype.h>
#include <vector>

#define IS_PLAYER_A(c)	(isupper(c))

#define BOARD_SIZE						10
#define NUM_OF_BOARD_ERR_MSGS		8

#define BOARD_DIMENSIONS_INVALID		"Board dimensions are invalid\n"
#define BOARD_DIMENSIONS_INVALID_IDX	0

using namespace std;


/**** Board Class *****/

class Board : public ErrorClass {
public:
	status_t parse();
	string getFilesPath();
	char getCharFromBoard(Point p);
	Board(string boardStringFromFile) : ErrorClass(NUM_OF_BOARD_ERR_MSGS), board(), numberOfPlayerAShips(0), numberOfPlayerBShips(0), boardStringFromFile(boardStringFromFile) {}
	int numberOfPlayerAShips, numberOfPlayerBShips;
	vector<Ship*> shipListA, shipListB;
private:
	char board[BOARD_SIZE][BOARD_SIZE];
	string boardStringFromFile;
	void setCharOnBoard(Point p, char val);
	bool checkAdjacentShips(const Ship &ship);
	bool checkSurroundingPoint(const Ship &ship, Point surroundingPoint);
	status_t const isBoardValid( const char (&parsedBoard)[BOARD_SIZE][BOARD_SIZE]);
	std::istream& Board::safeGetline(std::istream& is, std::string& t);
};
