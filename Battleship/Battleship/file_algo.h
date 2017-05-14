#pragma once

#include "battle.h"
#include "macros.h"
#include "file_parser.h"
#include "point.h"
#include "state.h"
#include <queue>

class FileAlgo : public IBattleshipGameAlgo
{
public:
	FileAlgo();
	//virtual Point getNextPotentialPoint();
	//virtual  Point * potentialLegalMoves();
	int getPotentialMovesSize();
	void setBoard(int player, const char** board, int numRows, int numCols) override;
	bool init(const std::string& path) override; //TBD
	std::pair<int, int> attack() ; // ask player for his move
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	void initPotentialMoves(const vector<Ship*>& shipList);
	char *board[BOARD_SIZE];

private:
	std::vector<std::string> split(const std::string &s, char delim);
	void processLine(const string & line, int whosTurn);
	void loadFromAttackFile(const string & attackPath, int whosTurn);

	char _board[BOARD_SIZE][BOARD_SIZE];
	int nextPointIndex = 0;
	Point *potentialMoves;
	
	int potentialMovesSize = 0;
	int player;
	bool isPointLegal[BOARD_SIZE][BOARD_SIZE];
	void initIsPointLegal(const vector<Ship*> &shipList);
	//vector<pair<int, int>> attackerDummy;
	std::queue<Point> attacker;

	// State

};
