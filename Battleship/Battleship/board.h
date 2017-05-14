#pragma once

#include <string>
#include "error_class.h"
#include "status.h"
#include "point.h"
#include "ships.h"
#include <ctype.h>
#include <vector>

#include "macros.h"


#define NUM_OF_BOARD_ERR_MSGS			NUMBER_OF_BOARD_MSGS

#define BOARD_DIMENSIONS_INVALID		"Board dimensions are invalid\n"
#define BOARD_DIMENSIONS_INVALID_IDX	0

using namespace std;


/**** Board Class *****/

class Board : public ErrorClass {
public:
	status_t parse();
	string getFilesPath();
	
	char getCharFromBoard(int x, int y) const;
	Board(string boardStringFromFile);
	
	int numberOfPlayerAShips, numberOfPlayerBShips;
	vector<Ship*> shipListA, shipListB;
	char *board[BOARD_SIZE];

private:
	char _board[BOARD_SIZE][BOARD_SIZE];
	string boardStringFromFile;
	void setCharOnBoard(Point p, char val);
	bool checkAdjacentShips(const Ship &ship);
	bool checkSurroundingPoint(const Ship &ship, Point surroundingPoint);
	status_t const isBoardValid(char **parsedBoard);
	std::istream& Board::safeGetline(std::istream& is, std::string& t);
};
