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
//#include "display.h"
#include <stdlib.h>

#define IS_SQUARE_DOWN(c)				(c == SQUARE_BOMBED_MISS_SYMBOL || c == SQUARE_BOMBED_HIT_SYMBOL)

enum class Winner
{
	PlayerA = PLAYER_A,
	PlayerB = PLAYER_B,
	Tie = PLAYER_B + 1
};

struct BattleScore
{
	Winner winner;
	int playerAPoints;
	int playerBPoints;
};

class Battle {
public:
	//Battle(Display &display, CmdParser cmd) : display(display), delay(cmd.getDelay()){}
	Battle(int _whosTurn) : whosTurn(_whosTurn), numOfSquareA(0), numOfSquareB(0) {}
	void setBoard(int player, const Board &board, IBattleshipGameAlgo * algo);
	Coordinate attack(IBattleshipGameAlgo &algo); // ask player for his move
	void notifyOnAttackResult(int player, Coordinate p, AttackResult result);
	virtual bool init(const std::string& path);	//TBD
	//ALGO_API IBattleshipGameAlgo* GetAlgorithm() override;
	BattleScore War(const Board &board, IBattleshipGameAlgo* algoA,  IBattleshipGameAlgo* algoB);      // note: no longger need here path to files!

private:
	//std::vector<HINSTANCE> dllList;
	//std::vector<IBattleshipGameAlgo *> playersList;

	char A_Board[BOARD_SIZE][BOARD_SIZE], B_Board[BOARD_SIZE][BOARD_SIZE];
	int numOfSquareA, numOfSquareB; 
	string filesPath;
	int whosTurn;
	void setWhosTurn(int turn);
	//Display &display;
	//IBattleshipGameAlgo *algoA, *algoB;
	
	uintmax_t delay;
	bool printBoardActive;
	IBattleshipGameAlgo *algoA, *algoB;
	//bool loadDllFiles(const string& path, const Board &board);
};
