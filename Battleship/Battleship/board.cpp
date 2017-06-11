#include "board.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "status.h"
#include "ships.h"
#include "macros.h"
#include "battle_utils.h"

#define NUM_OF_SHIPS_PER_PLAYER	5
#define IS_POINT_IN_POINT_LIST(pointList, pointListSize, point)	(std::find(pointList, pointList + pointListSize, point) != (pointList + pointListSize))
#define IS_SHIP_IN_POINT_LIST(ship, point)	IS_POINT_IN_POINT_LIST(ship.pointList, ship.size, point)

#define BOARD_DIMENSIONS_DELIMITER	"x"
/***************Public Methods***************/

Board::Board(string boardStringFromFile) : ErrorClass(NUM_OF_BOARD_ERR_MSGS), numberOfPlayerAShips(0), numberOfPlayerBShips(0), boardStringFromFile(boardStringFromFile)
{
	std::istringstream f(boardStringFromFile);
	std::string line;
	size_t pos = 0;
	vector<string> dimensions;

	//Parse first line - dimensions
	//std::getline(f, line); 
	safeGetline(f, line);
	std::cout << line << std::endl;

	// Add delimiter at the end of the line in order to work properly
	line += BOARD_DIMENSIONS_DELIMITER;

	std::string delimiter = BOARD_DIMENSIONS_DELIMITER;
	std::string token;
	while ((pos = line.find(delimiter)) != std::string::npos) {
		token = line.substr(0, pos);
		dimensions.push_back(token);
		line.erase(0, pos + delimiter.length());
	}

	// throw exception if not enough dimensions given
	if (dimensions.size() < 3)
	{
		DEBUG_PRINT("Only %d dimensions found\n", (int)dimensions.size());
		throw BoardBadDimensions();
	}

	// Parse dimensions
	try
	{
		_cols = std::stoi(dimensions[0]);
		_rows = std::stoi(dimensions[1]);
		_depth = std::stoi(dimensions[2]);
	}
	catch (...)
	{
		DEBUG_PRINT("Unable to parse dimension provided, make sure you provide an integer");
		throw BoardBadDimensions();
	}
	
	// Init board - a dynamic 3 dimensional array
	board = alloc3dArray<char>(_rows, _cols, _depth);	
}

// Overloading of Board constructor - if no parsing

Board::Board(int rows, int cols, int depth) : ErrorClass(NUM_OF_BOARD_ERR_MSGS), numberOfPlayerAShips(0), numberOfPlayerBShips(0)
{
	_rows = rows;
	_cols = cols;
	_depth = depth;
	board = alloc3dArray<char>(rows, cols, depth);
}

Board::~Board()
{
	delete board;
	// TODO: must delete board 3d array!!!
}

char Board::charAt(Coordinate c) const
{
	return board[c.row][c.col][c.depth];
}

status_t Board::parse()
{
	// row list for eeach depth
	//vector<string> rowList[_depth];

	
	vector<vector<string>> rowList;
	size_t i;
	uint32_t x, y;
	size_t lineNumber = 0;
	string t;
	status_t status;
	istringstream ifs = istringstream(this->boardStringFromFile);

	// make row list for each depth
	rowList.resize(depth());

	y = 0;

	// Skip first 2 lines - already read and parsed by constructor
	// Checking is just a precaution, should have already raised exception in constructor in case of error
	
	for (size_t i = 0; i < 2; i++)
	{
		if (safeGetline(ifs, t).eof()) {
			return STATUS_INVALID_BOARD;
		}
	}
	
	/*
	// Skip first line - already parsed by constructor
	// Checking is just a precaution, should have already raised exception in constructor in case of error
	if (safeGetline(ifs, t).eof()) {
		return STATUS_INVALID_BOARD;
	}
	*/


	// Read one depth at a time
	for (size_t d = 0; d < depth(); d++)
	{

		

		// Read row lines and append to rowList[d]
		// Now rowList[d] will be a list (vector) of rows for depth d 
		while (!safeGetline(ifs, t).eof())
		{
			rowList[d].push_back(t);
			if (++lineNumber == rows()) break;
		}

		// If not enough lines
		if (lineNumber < rows())
		{
			addErrorMsg(BOARD_DIMENSIONS_INVALID_IDX, BOARD_DIMENSIONS_INVALID);
			return STATUS_INVALID_BOARD;
		}

		// Read from rowList to board
		for (x = 0; x < rowList[d].size(); x++)
		{
			if (x == 9 && d == 1 ) {
				d = d;
			}
			// Iterate cols 
			for (i = 0; i < rowList[d][x].size(); i++)
			{
				if (rowList[d][x][i] == ' '
					|| rowList[d][x][i] == 'b'
					|| rowList[d][x][i] == 'B'
					|| rowList[d][x][i] == 'p'
					|| rowList[d][x][i] == 'P'
					|| rowList[d][x][i] == 'm'
					|| rowList[d][x][i] == 'M'
					|| rowList[d][x][i] == 'd'
					|| rowList[d][x][i] == 'D')
				{
					setCharOnBoard(Point(x, y++, d), rowList[d][x][i]);


				}
				else {
					setCharOnBoard(Point(x, y++, d), ' ');
				}

				// Discard any tokens after BOARD_SIZE have been found
				if (y == cols()) break;
			}

			// If not enough chars in line x
			if (y != cols())
			{
				addErrorMsg(BOARD_DIMENSIONS_INVALID_IDX, BOARD_DIMENSIONS_INVALID);
				return STATUS_INVALID_BOARD;
			}

			y = 0;
		}

		// Before next depth we must read an empty line
		if (safeGetline(ifs, t).eof() && d != depth() - 1) {
			addErrorMsg(BOARD_DIMENSIONS_INVALID_IDX, BOARD_DIMENSIONS_INVALID);
			return STATUS_INVALID_BOARD;
		}

		lineNumber = 0;
	}
	// Check board for errors and collect error messages
	status = isBoardValid(board);

	return status;
}



/***************Private Methods***************/

bool Board::isPartOfFoundList(Point point, const vector<Ship*> &shipListA, const vector<Ship*> &shipListB)
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

status_t const Board::isBoardValid(char ***parsedBoard)
{
	Ship *ship;
	char part, nextPart;
	int i, j;

	for (int d = 0; d < depth(); d++)
	{
		for (int x = 0; x < rows(); x++)
		{
			for (int y = 0; y < cols(); y++)
			{

				part = parsedBoard[x][y][d];

				// If already part of a found ship - skip this point
				if (isPartOfFoundList(Point(x, y, d), shipListA, shipListB)) {
					continue;
				}

				// If found part of piece
				if (part != ' ')
				{
					ship = charToShip(part);
					ship->pointList[0] = Point(x, y, d);

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

						delete ship;
					}
					else {

						// Try vertical
						for (j = 1; j < ship->size; j++)
						{
							if (x + j >= rows()) {
								//DEBUG_PRINT("Ship %c too small\n", ship->charSymbol);
								//this->addErrorMsg(ship->msgWrongSizeIdx, ship->msgWrongSize);
								//delete ship;
								// Finished column, go to next row
								break;
							}

							nextPart = parsedBoard[x + j][y][d];

							//Correct part
							if (nextPart == ship->charSymbol) {
								ship->pointList[j] = Point(x + j, y, d);
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
							if (y + i >= cols()) {
								DEBUG_PRINT("Ship %c too small\n", ship->charSymbol);
								this->addErrorMsg(ship->msgWrongSizeIdx, ship->msgWrongSize);
								delete ship;
								// Finished row, go to next row
								break;
							}
							nextPart = parsedBoard[x][y + i][d];

							//Correct part
							if (nextPart == ship->charSymbol) {
								ship->pointList[i] = Point(x, y + i, d);
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
								// Ship found go to next iteration
								continue;
							}

						}

						// Try depth
						for (i = 1; i < ship->size; i++)
						{
							if (d + i >= depth()) {
								DEBUG_PRINT("Ship %c too small\n", ship->charSymbol);
								this->addErrorMsg(ship->msgWrongSizeIdx, ship->msgWrongSize);
								delete ship;
								// Finished row, go to next row
								break;
							}
							nextPart = parsedBoard[x][y][d + i];

							//Correct part
							if (nextPart == ship->charSymbol) {
								ship->pointList[i] = Point(x, y, d + i);
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
								// Ship found go to next iteration
								continue;
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
	char c = board[surroundingPoint.row][surroundingPoint.col][surroundingPoint.depth];
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
		if ((int)shipPoint.row + 1 < rows()) {
			if (!checkSurroundingPoint(ship, Point(shipPoint.row + 1, shipPoint.col, shipPoint.depth))) ret = false;
		}
		if ((int)shipPoint.row - 1 >= 0) {
			if (!checkSurroundingPoint(ship, Point(shipPoint.row - 1, shipPoint.col, shipPoint.depth))) ret = false;
		}
		if ((int)shipPoint.col + 1 < cols()) {
			if (!checkSurroundingPoint(ship, Point(shipPoint.row, shipPoint.col + 1, shipPoint.depth))) ret = false;
		}
		if ((int)shipPoint.col - 1 >= 0) {
			if (!checkSurroundingPoint(ship, Point(shipPoint.row, shipPoint.col - 1, shipPoint.depth))) ret = false;
		}
		if ((int)shipPoint.depth + 1 < depth()) {
			if (!checkSurroundingPoint(ship, Point(shipPoint.row, shipPoint.col, shipPoint.depth + 1))) ret = false;
		}
		if ((int)shipPoint.depth - 1 >= 0) {
			if (!checkSurroundingPoint(ship, Point(shipPoint.row, shipPoint.col, shipPoint.depth - 1))) ret = false;
		}
		
	}

	return ret;
}


void Board::setCharOnBoard(Point p, char val)
{
	board[p.row][p.col][p.depth] = val;
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


