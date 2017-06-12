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
#include <stdlib.h>
#include <Windows.h>

#define IS_SQUARE_DOWN(c)				(c == SQUARE_BOMBED_MISS_SYMBOL || c == SQUARE_BOMBED_HIT_SYMBOL)


class Battle {
public:
	Battle(CmdParser cmd) : delay(cmd.getDelay()){}
	Battle() {}
	void setBoard(int player, const Board &board, IBattleshipGameAlgo * algo);
	Coordinate attack(IBattleshipGameAlgo &algo); // ask player for his move
	void notifyOnAttackResult(int player, Coordinate p, AttackResult result);
	virtual bool init(const std::string& path); //TBD
	//ALGO_API IBattleshipGameAlgo* GetAlgorithm() override;
	int War(const string &path, const Board &board);

private:
	std::vector<HINSTANCE> dllList;
	std::vector<IBattleshipGameAlgo *> playersList;

	char A_Board[BOARD_SIZE][BOARD_SIZE], B_Board[BOARD_SIZE][BOARD_SIZE];
	int numOfSquareA = 0, numOfSquareB = 0; 
	string filesPath;
	int whosTurn;
	void setWhosTurn(int turn);
	//IBattleshipGameAlgo *algoA, *algoB;
	
	uintmax_t delay;
	bool printBoardActive;
	bool loadDllFiles(const string& path, const Board &board);
};
