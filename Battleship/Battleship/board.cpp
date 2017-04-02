#include "board.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

/***************Public Methods***************/

Board_t Board::parse()
{
	vector<string> rowList;
	size_t i;
	uint32_t x, y;
	size_t lineNumber = 0;
	string t;

	istringstream ifs = istringstream(this->boardStringFromFile);

	// Read BOARD_SIZE lines and uppend to rowList
	while (!safeGetline(ifs, t).eof())
	{
		rowList.push_back(t);
		if (++lineNumber == BOARD_SIZE) break;
	}

	// If not enough lines
	if (lineNumber < BOARD_SIZE)
	{
		errorMsgs[BOARD_DIMENSIONS_INVALID_IDX] = new string(BOARD_DIMENSIONS_INVALID);
		return BOARD_FILE_NOT_VALID;
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
				setCharOnBoard(Point(x, ++y), rowList[x][i]);
				
				// Discard any tokens after BOARD_SIZE have been found
				if (y == BOARD_SIZE) break;
			}

			// If not enough valid chars in line x
			if (y != BOARD_SIZE)
			{
				errorMsgs[BOARD_DIMENSIONS_INVALID_IDX] = new string(BOARD_DIMENSIONS_INVALID);
				return BOARD_FILE_NOT_VALID;
			}
		}

		y = 0;
	}

	//TODO: Now must check board for errors and collect error messages
}


/***************Private Methods***************/

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