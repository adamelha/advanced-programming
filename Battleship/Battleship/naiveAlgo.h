#pragma once
#include "State.h";
#define arrSize 10

class naiveAlgo :  public IBattleshipGameAlgo
{
public:
	//naiveAlgo() :State();
	virtual Point getNextPotentialPoint() ;
	virtual  Point * potentialLegalMoves() ;
	int getPotentialMovesSize();
	void setBoard(int player ,const char** board, int numRows, int numCols) override;
	virtual bool init(const std::string& path) override; //TBD
	std::pair<int, int> attack() override; // ask player for his move
	virtual void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	void initPotentialMoves(const vector<Ship*>& shipList);
//	Point * getPotentialMoves();

private:
	int nextPointIndex = 0;
	Point potentialMoves[100];
	int potentialMovesSize = 0;
	bool isPointLegal[arrSize][arrSize];
	void initIsPointLegal(const vector<Ship*> &shipList);
	//void initPotentialMovesSize();
	//void initPotentialMoves(const vector<Ship*>& shipList);
	//void initIsPointLegal(const vector<Ship*> &shipList);
	



};
