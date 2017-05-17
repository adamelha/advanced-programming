#pragma once

#include "state.h";
#include "macros.h"
#include "battle_utils.h"
#include "rand_pool.h"
#include "ships.h"
class SmartAlgo : public IBattleshipGameAlgo
{
public:
	//NaiveAlgo () :State();
	SmartAlgo();
	~SmartAlgo();
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
	RandPool *randPool;
	RandPool *randPoolTemp;
	Point currentAttackArr[SHIP_SIZE_MAX];
	int currentAttackIndex;

	int potentialMovesSize = 0;
	bool isPointLegal[BOARD_SIZE][BOARD_SIZE];
	Point nextPointFrom2PointsDirPlus();
	Point nextPointFrom2PointsDirMinus();
	Point getLowestPointIncurrentAttackArr();
	Point getHighestPointIncurrentAttackArr();


	void initIsPointLegal();
	bool isPointPartOfShip(int x, int y);
	void changeEnvalopPointsToFalse(bool arr[][BOARD_SIZE], int x, int y);
	player_is lastPlayerFormNotifyed = player_A;
	Point lastPointFormNotifyed;
	int player;
	Point directions[4];
	int directionsIndex;
	int dirState2Points;
	//void initPotentialMovesSize();
	//void initPotentialMoves(const vector<Ship*>& shipList);
	//void initIsPointLegal(const vector<Ship*> &shipList);

};
