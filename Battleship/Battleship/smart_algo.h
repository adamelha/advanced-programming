#pragma once

#include "macros.h"
#include "battle_utils.h"
#include "rand_pool.h"
#include "ships.h"
#include <iostream>

#define UP_POINT	(Point(0, 1, 0))
#define DOWN_POINT	(Point(0, -1, 0))
#define RIGHT_POINT	(Point(1, 0, 0))
#define LEFT_POINT	(Point(-1, 0, 0))
#define IN_POINT	(Point(0, 0, 1))
#define OUT_POINT	(Point(0, 0, -1))
#define DIRECTIONS_FINAL_INDEX	(5)

class SmartAlgo : public IBattleshipGameAlgo
{
public:
	//NaiveAlgo () :State();
	SmartAlgo();
	~SmartAlgo();
	virtual Point getNextPotentialPoint();
	virtual  Point * potentialLegalMoves();
	int getPotentialMovesSize();
	void setPlayer(int player) override;
	void setBoard(const BoardData & board) override;
	//virtual bool init(const std::string& path) override; //TBD
	Coordinate attack() override; // ask player for his move
	virtual void notifyOnAttackResult(int player, Coordinate move, AttackResult result) override;
	void initPotentialMoves();
	//char *board[BOARD_SIZE];
	char ***board;
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
	bool isPointPartOfShip(Point p);
	void changeEnvalopPointsToFalse(bool arr[][BOARD_SIZE], int x, int y);
	player_is lastPlayerFormNotifyed = player_A;
	Point lastPointFormNotifyed;
	int player;
	Point directions[DIRECTIONS_FINAL_INDEX + 1];
	int directionsIndex;
	int dirState2Points;
	void setBoard(int rows, int cols, int depth);
	int rows, cols, depth;
	//void initPotentialMovesSize();
	//void initPotentialMoves(const vector<Ship*>& shipList);
	//void initIsPointLegal(const vector<Ship*> &shipList);

};
