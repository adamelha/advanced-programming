#include "board.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "status.h"
#include "ships.h"
#include "macros.h"


#define NUM_OF_SHIPS_PER_PLAYER	5
#define IS_POINT_IN_POINT_LIST(pointList, pointListSize, point)	(std::find(pointList, pointList + pointListSize, point) != (pointList + pointListSize))
#define IS_SHIP_IN_POINT_LIST(ship, point)	IS_POINT_IN_POINT_LIST(ship.pointList, ship.size, point)


/***************Public Methods***************/

Board::Board(string boardStringFromFile) : ErrorClass(NUM_OF_BOARD_ERR_MSGS), numberOfPlayerAShips(0), numberOfPlayerBShips(0), boardStringFromFile(boardStringFromFile)
{
	// Init board to be array of char pointers, each pointing to a row in _board matrix (avoids dynamic allocation)
	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		board[i] = &(_board[i][0]);
	}
}
status_t Board::parse()
{
	vector<string> rowList;
	size_t i;
	uint32_t x, y;
	size_t lineNumber = 0;
	string t;
	status_t status;
	istringstream ifs = istringstream(this->boardStringFromFile);

	y = 0;

	// Read BOARD_SIZE lines and uppend to rowList
	while (!safeGetline(ifs, t).eof())
	{
		rowList.push_back(t);
		if (++lineNumber == BOARD_SIZE) break;
	}

	// If not enough lines
	if (lineNumber < BOARD_SIZE)
	{
		addErrorMsg(BOARD_DIMENSIONS_INVALID_IDX, BOARD_DIMENSIONS_INVALID);
		return STATUS_INVALID_BOARD;
	}

	// Read from rowList to board
	for (x = 0; x < rowList.size(); x++)
	{
		for (i = 0; i < rowList[x].size(); i++)
		{
			if (rowList[x][i] == ' '
				|| rowList[x][i] == 'b'
				|| rowList[x][i] == 'B'
				|| rowList[x][i] == 'p'
				|| rowList[x][i] == 'P'
				|| rowList[x][i] == 'm'
				|| rowList[x][i] == 'M'
				|| rowList[x][i] == 'd'
				|| rowList[x][i] == 'D')
			{
				setCharOnBoard(Point(x, y++), rowList[x][i]);

				
			}
			else {
				setCharOnBoard(Point(x, y++), ' ');
			}

			// Discard any tokens after BOARD_SIZE have been found
			if (y == BOARD_SIZE) break;
		}

		// If not enough valid chars in line x
		if (y != BOARD_SIZE)
		{
			addErrorMsg(BOARD_DIMENSIONS_INVALID_IDX, BOARD_DIMENSIONS_INVALID);
			return STATUS_INVALID_BOARD;
		}

		y = 0;
	}

	// Check board for errors and collect error messages
	status = isBoardValid(board);

	return status;
}


/***************Private Methods***************/

static bool isPartOfFoundList(Point point, const vector<Ship*> &shipListA, const vector<Ship*> &shipListB)
{
	size_t i;

	for (i = 0; i < shipListA.size(); i++)
	{
		if (IS_POINT_IN_POINT_LIST(shipListA[i]->pointList, shipListA[i]->size, point)) {
			return true;
		}
	}
	for (i = 0; i < shipListB.size(); i++)
	{
		if (IS_POINT_IN_POINT_LIST(shipListB[i]->pointList, shipListB[i]->size, point)) {
			return true;
		}
	}
	return false;
}

status_t const Board::isBoardValid(char **parsedBoard)
{
	Ship *ship;
	char part, nextPart;
	int i, j;

	for (int x = 0; x < BOARD_SIZE; x++)
	{
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			part = parsedBoard[x][y];

			// If already part of a found ship - skip this point
			if (isPartOfFoundList(Point(x, y), shipListA, shipListB)) {
				continue;
			}

			// If found part of piece
			if (part != ' ')
			{
				ship = charToShip(part);
				ship->pointList[0] = Point(x, y);

				// If ship of size 1 - no need to search for vertical and horizontal
				if (ship->size == 1) {
					if (checkAdjacentShips(*ship)) {
						if (IS_PLAYER_A(ship->charSymbol)) {
							shipListA.push_back(ship);
						}
						else {
							shipListB.push_back(ship);
						}
					}
				}
				else {

					// Try vertical
					for (j = 1; j < ship->size; j++)
					{
						if (x + j >= BOARD_SIZE) {
							//DEBUG_PRINT("Ship %c too small\n", ship->charSymbol);
							//this->addErrorMsg(ship->msgWrongSizeIdx, ship->msgWrongSize);
							//delete ship;
							// Finished column, go to next row
							break;
						}

						nextPart = parsedBoard[x + j][y];

						//Correct part
						if (nextPart == ship->charSymbol) {
							ship->pointList[j] = Point(x + j, y);
							continue;
						}
						// Wrong Part
						else
						{
							//DEBUG_PRINT("Ship %c too small\n", ship->charSymbol);
							//this->addErrorMsg(ship->msgWrongSizeIdx, ship->msgWrongSize);
							//delete ship;
							break;
						}
					}

					// Found complete vertical ship 
					if (j == ship->size)
					{
						// If success!
						if (checkAdjacentShips(*ship)) {
							if (IS_PLAYER_A(ship->charSymbol)) {
								shipListA.push_back(ship);
							}
							else {
								shipListB.push_back(ship);
							}
							// Ship found go to next iteration
							continue;
						}

					}


					// Try horizontal
					for (i = 1; i < ship->size; i++)
					{
						if (y + i >= BOARD_SIZE) {
							DEBUG_PRINT("Ship %c too small\n", ship->charSymbol);
							this->addErrorMsg(ship->msgWrongSizeIdx, ship->msgWrongSize);
							delete ship;
							// Finished row, go to next row
							break;
						}
						nextPart = parsedBoard[x][y + i];

						//Correct part
						if (nextPart == ship->charSymbol) {
							ship->pointList[i] = Point(x, y + i);
							//continue;
						}
						// Wrong Part
						else
						{
							DEBUG_PRINT("Ship %c too small\n", ship->charSymbol);
							this->addErrorMsg(ship->msgWrongSizeIdx, ship->msgWrongSize);
							delete ship;
							break;
						}
					}
						// Found complete horizontal ship 
					if (i == ship->size)
					{
						// If success!
						if (checkAdjacentShips(*ship)) {
							if (IS_PLAYER_A(ship->charSymbol)) {
								shipListA.push_back(ship);
							}
							else {
								shipListB.push_back(ship);
							}
						}

					}
					
				}
			}
		}
	}

	if (shipListA.size() > NUM_OF_SHIPS_PER_PLAYER) {
		this->addErrorMsg(MSG_MANY_SHIPS_A_IDX, MSG_MANY_SHIPS_A);
	}
	if (shipListB.size() > NUM_OF_SHIPS_PER_PLAYER) {
		this->addErrorMsg(MSG_MANY_SHIPS_B_IDX, MSG_MANY_SHIPS_B);
	}

	if (shipListA.size() < NUM_OF_SHIPS_PER_PLAYER) {
		this->addErrorMsg(MSG_FEW_SHIPS_A_IDX, MSG_FEW_SHIPS_A);
	}
	if (shipListB.size() < NUM_OF_SHIPS_PER_PLAYER) {
		this->addErrorMsg(MSG_FEW_SHIPS_B_IDX, MSG_FEW_SHIPS_B);
	}

	if (errorMsgsExist()) {
		return STATUS_INVALID_BOARD;
	}

	return STATUS_OK;
}

bool Board::checkSurroundingPoint(const Ship &ship, Point surroundingPoint)
{
	bool ret = true;
	char c = board[surroundingPoint.x][surroundingPoint.y];
	if (c != ' ') {
		// If surrounding point is same type of ship, and is not part of this ship - wrong size error
		if (c == ship.charSymbol && !IS_SHIP_IN_POINT_LIST(ship, surroundingPoint)) {
			this->addErrorMsg(ship.msgWrongSizeIdx, ship.msgWrongSize);
			ret = false;
		}
		// If surrounding point is same type of ship and is part of this ship
		else if (c == ship.charSymbol) {
			ret = true;
		}
		// If surrounding point is different type of ship - adjacent ships error
		else {
			this->addErrorMsg(MSG_ADJACENT_SHIPS_IDX, MSG_ADJACENT_SHIPS);
		}
	}

	return ret;
}

// Check if any ships are adjacent to ship.
// If only an adjacent part of the same ship is found - set msgWrongSize error and return false.
// If only an adjacent part of a different ship is found - set msgAdjacentShips error and return true because we count this as a ship.
// If both situations occur - set both messages and return false.
// If no adjacent parts - return true.
bool Board::checkAdjacentShips(const Ship &ship)
{
	Point shipPoint;
	bool ret = true;
	for (size_t i = 0; i < ship.size; i++)
	{
		shipPoint = ship.pointList[i];

		// Check surrounding of shipPoint that is not part of the ship
		if ((int)shipPoint.x + 1 < BOARD_SIZE) {
			if (!checkSurroundingPoint(ship, Point(shipPoint.x + 1, shipPoint.y))) ret = false;
		}
		if ((int)shipPoint.x - 1 >= 0) {
			if (!checkSurroundingPoint(ship, Point(shipPoint.x - 1, shipPoint.y))) ret = false;
		}
		if ((int)shipPoint.y + 1 < BOARD_SIZE) {
			if (!checkSurroundingPoint(ship, Point(shipPoint.x, shipPoint.y + 1))) ret = false;
		}
		if ((int)shipPoint.y - 1 >= 0) {
			if (!checkSurroundingPoint(ship, Point(shipPoint.x, shipPoint.y - 1))) ret = false;
		}
		
	}

	return ret;
}


void Board::setCharOnBoard(Point p, char val)
{
	board[p.x][p.y] = val;
}

//TODO: Also treats a plain "\r" as an endline. Must be changed.
// Takes care of line endings - credit Stackoverflow: http://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf
// TODO: should probably be moved to a FileUtils class that Board and Attack classes inherit.
std::istream& Board::safeGetline(std::istream& is, std::string& t)
{
	t.clear();

	// The characters in the stream are read one-by-one using a std::streambuf.
	// That is faster than reading them one-by-one using the std::istream.
	// Code that uses streambuf this way must be guarded by a sentry object.
	// The sentry object performs various tasks,
	// such as thread synchronization and updating the stream state.

	std::istream::sentry se(is, true);
	std::streambuf* sb = is.rdbuf();

	for (;;) {
		int c = sb->sbumpc();
		switch (c) {
		case '\n':
			return is;
		case '\r':
			if (sb->sgetc() == '\n')
				sb->sbumpc();
			return is;
		case EOF:
			// Also handle the case when the last line has no line ending
			if (t.empty())
				is.setstate(std::ios::eofbit);
			return is;
		default:
			t += (char)c;
		}
	}
}



char Board::getCharFromBoard(int x, int y) const
{
	return this->board[x][y];
}

