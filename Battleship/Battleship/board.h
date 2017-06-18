#pragma once

#include <string>
#include "error_class.h"
#include "status.h"
#include "point.h"
#include "ships.h"
#include <ctype.h>
#include <vector>
#include "IBattleshipGameAlgo.h"
#include "macros.h"


#define NUM_OF_BOARD_ERR_MSGS			NUMBER_OF_BOARD_MSGS

#define BOARD_DIMENSIONS_INVALID		"Board dimensions are invalid\n"
#define BOARD_DIMENSIONS_INVALID_IDX	0

#define BOARD_DIMENSIONS_EXCEPTIONS		1

#define BOARD_NUMBER_OF_DIMENSIONS		3
#define BOARD_ROWS	10
#define BOARD_COLS	10
#define BOARD_DEPTH	10
using namespace std;

struct BoardBadDimensionsOrHasMoreBoardsInFile {};
//struct BoardNoMoreBoards {};

/**** Board Class *****/

class Board : public ErrorClass, public BoardData {
public:
	status_t parse();	
	Board(string boardStringFromFile);
	char charAt(Coordinate c) const;
	 
	Board(int rows, int cols, int depth);
	~Board();
	int numberOfPlayerAShips, numberOfPlayerBShips;
	vector<Ship*> shipListA, shipListB;
	char ***board;
	

private:
	char _board[BOARD_SIZE][BOARD_SIZE];
	string boardStringFromFile;
	void setCharOnBoard(Point p, char val);
	bool checkAdjacentShips(const Ship &ship);
	bool checkSurroundingPoint(const Ship &ship, Point surroundingPoint);
	status_t const isBoardValid(char ***parsedBoard);
	std::istream& Board::safeGetline(std::istream& is, std::string& t);
	bool isPartOfFoundList(Point point, const vector<Ship*> &shipListA, const vector<Ship*> &shipListB);
	int boardNumber;
};
