#include "file_algo.h"
#include "file_parser.h"
#include "battle_utils.h"

#define VALID_INT_TOKENS  (0 < stoi(tokens[0]) && stoi(tokens[0]) < 11 && 0 < stoi(tokens[1]) && stoi(tokens[1]) < 11)


/*****Public Functions*******/

FileAlgo::FileAlgo()
{
	// Init board to be array of char pointers, each pointing to a row in _board matrix (avoids dynamic allocation)
	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		board[i] = &(_board[i][0]);
	}
}

void FileAlgo::setBoard(int player, const char ** board, int numRows, int numCols)
{
	squareType_e st;
	this->player = player;

	if (player)	//set Board to Player B
	{
		st = B_SQUARE;
	}
	else //set Board to Player A
	{
		st = A_SQUARE;
	}

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if (isBelongToBoard(board[i][j]) == st)
			{
				this->board[i][j] = board[i][j];
				//numOfSquareB += 1;
			}
			else
				this->board[i][j] = ' ';       // initialize board to 
		}
	}
}


bool FileAlgo::init(const std::string& path)
{
	status_t status = STATUS_OK;
	FileParser fp(path, (parseType_e)player);

	status = fp.parse();
	if (status != STATUS_OK) {
		fp.printErrorMsg();
		return false;
	}

	//set players attack vectors
	loadFromAttackFile(fp.getAttackFileName(), player);
	return true;
}


// return next move by dequeing from atteacker queue.
// return pair of OUT_OF_MOVES, OUT_OF_MOVES if no more moves available.
std::pair<int, int> FileAlgo::attack()
{
	if (attacker.empty())
	{
		return std::pair<int, int>(OUT_OF_MOVES, OUT_OF_MOVES);
	}
	Point p = attacker.front();
	attacker.pop();

	return p.toPair();
}

// Do nothing with notification. Just reads next move in the queue.
void FileAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{

}
/******Private Functions*******/

// split,processLine,loadFromAttackFile methods are from Tirgul 3 , with some minor changes

std::vector<std::string> FileAlgo::split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	std::stringstream strSt(s);
	std::string item;

	while (std::getline(strSt, item, delim)) //we could ommit the 'std::' notion from here.
	{
		elems.push_back(item); //push_back is a function of vector class, it inserted to the back of the vector.
	}

	return elems;
}



void FileAlgo::processLine(const string & line, int whosTurn)
{
	vector<string> tokens = split(line, ',');
	if (tokens.size() != 2 || !VALID_INT_TOKENS) 
	{
		return;
	}

	attacker.push(Point(stoi(tokens[0]) - 1, stoi(tokens[1]) - 1));
}

void FileAlgo::loadFromAttackFile(const string & attackPath, int whosTurn)
{
	ifstream fin(attackPath);			//creating an ifstream object and opening file in path attackPath 
	string line;
	while (getline(fin, line))		   //getline returns null if we get to end of file.
	{
		this->processLine(line, whosTurn);
	}
}

