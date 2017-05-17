#pragma once

#include "State.h";
#include "macros.h"
#include "battle_utils.h"


class NaiveAlgo : public IBattleshipGameAlgo
{
public:
	//NaiveAlgo () :State();
	NaiveAlgo();
	virtual Point getNextPotentialPoint();
	virtual  Point * potentialLegalMoves();
	int getPotentialMovesSize();
	void setBoard(int player, const char** board, int numRows, int numCols) override;
	virtual bool init(const std::string& path) override; //TBD
	std::pair<int, int> attack() override; // ask player for his move
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	void initPotentialMoves();
	char *board[BOARD_SIZE];
	//	Point * getPotentialMoves();

private:
	char _board[BOARD_SIZE][BOARD_SIZE];
	int nextPointIndex = 0, numOfSqares = 0;
	Point potentialMoves[100];
	int potentialMovesSize = 0;
	bool isPointLegal[BOARD_SIZE][BOARD_SIZE];
	void initIsPointLegal();
	bool isPointPartOfShip(int x, int y);
	void changeEnvalopPointsToFalse(bool arr[][BOARD_SIZE], int x, int y);
	player_is lastPlayerFormNotifyed = player_A;
	Point lastPointFormNotifyed;

	//void initPotentialMovesSize();
	//void initPotentialMoves(const vector<Ship*>& shipList);
	//void initIsPointLegal(const vector<Ship*> &shipList);

};
