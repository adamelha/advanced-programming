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
#define SQUARE_DOWN_SYMBOL	'\n'

class Battle : public IBattleshipGameAlgo {
public:
	Battle(Display &display, CmdParser cmd) : display(display), delay(cmd.getDelay()){}
	void setBoard(int player, const char** board, int numRows, int numCols) override;
	std::pair<int, int> attack() override; // ask player for his move
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	virtual bool init(const std::string& path) override; //TBD
	//ALGO_API IBattleshipGameAlgo* GetAlgorithm() override;
	int War(const FileParser &fileParser, const Board &board);

private:

	char A_Board[BOARD_SIZE][BOARD_SIZE], B_Board[BOARD_SIZE][BOARD_SIZE];
	vector<pair<int, int>> A_Atacker, B_Atacker;
	int numOfSquareA = 0, numOfSquareB = 0; 
	string filesPath;
	int whosTurn;
	void setFilePath(string path);
	void setWhosTurn(int turn);
	void processLine(const string& line, int whosTurn);
	void loadFromAttackFile(const string& attackPath, int whosTurn);
	Display &display;
	uintmax_t delay;
	bool printBoardActive;
};
