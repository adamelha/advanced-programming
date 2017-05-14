#pragma once

#include "cmd_parser.h"
#include "file_parser.h"
#include "board.h"
#include "status.h"
#include "IBattleshipGameAlgo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "macros.h"
#include "display.h"

#define IS_SQUARE_DOWN(c)				(c == SQUARE_BOMBED_MISS_SYMBOL || c == SQUARE_BOMBED_HIT_SYMBOL)


class Battle {
public:
	Battle(Display &display, CmdParser cmd, IBattleshipGameAlgo &algoA, IBattleshipGameAlgo &algoB) : display(display), delay(cmd.getDelay()), algoA(algoA), algoB(algoB){}
	void setBoard(int player, const char** board, int numRows, int numCols, IBattleshipGameAlgo &algo);
	std::pair<int, int> attack(); // ask player for his move
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result);
	virtual bool init(const std::string& path); //TBD
	//ALGO_API IBattleshipGameAlgo* GetAlgorithm() override;
	int War(const string &path, const Board &board);

private:

	char A_Board[BOARD_SIZE][BOARD_SIZE], B_Board[BOARD_SIZE][BOARD_SIZE];
	int numOfSquareA = 0, numOfSquareB = 0; 
	string filesPath;
	int whosTurn;
	void setWhosTurn(int turn);
	Display &display;
	IBattleshipGameAlgo &algoA, &algoB;
	uintmax_t delay;
	bool printBoardActive;
};
