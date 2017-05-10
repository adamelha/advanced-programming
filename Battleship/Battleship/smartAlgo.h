#pragma once

#include "State.h";
#include <stdlib.h>
#include <time.h>
#define arrSize 10

class smartAlgo : public IBattleshipGameAlgo
{
public:
	//naiveAlgo() :State();
	virtual Point getNextPotentialPoint();
	virtual  Point * potentialLegalMoves();
	int getPotentialMovesSize();
	void setBoard(int player, const char** board, int numRows, int numCols) override;
	std::pair<int, int> attack() override; // ask player for his move
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	virtual bool init(const std::string& path) override;
	void initPotentialMoves(const vector<Ship*>& shipList);
	//	Point * getPotentialMoves();

private:

	int nextPointIndex = 0;
	Point potentialMoves[100];
	int potentialMovesSize = 0;
	bool isPointLegal[arrSize][arrSize];
	bool strike = false;
	bool localEnv[4];
	int localEnvIndex = 0;
	Point hitPoint;

	//Point retLocalEnv
	Point * PointerForPotentialMove[arrSize][arrSize];
	void initIsPointLegal(const vector<Ship*> &shipList);
	void changeEnvalopPointsToFalse(bool arr[][arrSize], int i, int j);





};
