#pragma once

#include "cmd_parser.h"
#include "file_parser.h"
#include "board.h"
#include "status.h"
#include "IBattleshipGameAlgo.h"
#include <iostream>
#include <fstream>
#include <sstream>

#define ARR_SIZE	10



class Battle : public IBattleshipGameAlgo {
public:
	virtual void setBoard(const char** board, int numRows, int numCols) override;
	virtual std::pair<int, int> attack() override; // ask player for his move
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	int War(FileParser fileParser, Board board);

private:

	char A_Board[ARR_SIZE][ARR_SIZE], B_Board[ARR_SIZE][ARR_SIZE];
	vector<pair<int, int>> A_Atacker, B_Atacker;
	int numOfSquareA = 0, numOfSquareB = 0; 
	string filesPath;
	

	int whosTurn;
	void setFilePath(string path);
	void setWhosTurn(int turn);
	void setLineOrColumn(char board[][ARR_SIZE], int size, Point* p, bool isVertical, char symbol);
	void setAttacker(string attackStr, int whosTurn);
	//vector<pair<int, int>> A_Atacker, B_Atacker;
	void processLine(const string& line, int whosTurn);
	void loadFromAttackFile(const string& attackPath, int whosTurn);
	






};
