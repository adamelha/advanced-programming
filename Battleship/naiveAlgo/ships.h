#pragma once
#include "point.h"
#include <string>
#include <vector>
using namespace std;

#define SHIP_SIZE_B				1
#define SHIP_SIZE_P				2
#define SHIP_SIZE_M				3
#define SHIP_SIZE_D				4

#define SHIP_POINTS_B			2
#define SHIP_POINTS_P			3
#define SHIP_POINTS_M			7
#define SHIP_POINTS_D			8

#define NUM_OF_DIFFERENT_SHIPS	4
#define NUM_OF_PLAYERS			2

#define MSG_MANY_SHIPS_A		"Too many ships for player A\n"
#define MSG_MANY_SHIPS_A_IDX	(NUM_OF_DIFFERENT_SHIPS * NUM_OF_PLAYERS)
#define MSG_FEW_SHIPS_A			"Too few ships for player A\n"
#define MSG_FEW_SHIPS_A_IDX		(MSG_MANY_SHIPS_A_IDX + 1)
#define MSG_MANY_SHIPS_B		"Too many ships for player B\n"
#define MSG_MANY_SHIPS_B_IDX	(MSG_FEW_SHIPS_A_IDX + 1)
#define MSG_FEW_SHIPS_B			"Too few ships for player B\n"
#define MSG_FEW_SHIPS_B_IDX		(MSG_MANY_SHIPS_B_IDX + 1)

#define MSG_ADJACENT_SHIPS		"Adjacent Ships on Board\n"
#define MSG_ADJACENT_SHIPS_IDX	(MSG_FEW_SHIPS_B_IDX + 1)
#define NUMBER_OF_BOARD_MSGS	(MSG_ADJACENT_SHIPS_IDX + 1)


class Ship {
public:
	Ship(size_t sizeOfShip, int pointsWorth) : pointList(new Point[sizeOfShip]), size(sizeOfShip), pointsWorth(pointsWorth), activePointsLeft(sizeOfShip) {}
	// Copy constructor
	Ship(const Ship &ship);
	// =operator
	Ship& operator=(const Ship& other);
	char player;
	char charSymbol;
	size_t size;
	Point *pointList;
	size_t msgWrongSizeIdx, msgTooManyShipsIdx, msgTooFewShipsIdx, msgAdjacentShipsIdx;
	string msgWrongSize;
	string msgTooManyShips;
	string msgTooFewShips;
	string msgAdjacentShips;
	void initErrorStrings();
	int getPointsWorth();
	int activePointsLeft; // Used for keeping track of ship status - when does it sink.
	~Ship() { delete[] pointList; }

protected:
	int pointsWorth;

};

class ShipB : public Ship {
public:
	ShipB() : Ship(SHIP_SIZE_B, SHIP_POINTS_B) {}
};

class ShipBPlayerA : public ShipB {
public:
	ShipBPlayerA();
};
class ShipBPlayerB : public ShipB {
public:
	ShipBPlayerB();
};

class ShipP : public Ship {
public:
	ShipP() : Ship(SHIP_SIZE_P, SHIP_POINTS_P) {}
};

class ShipPPlayerA : public ShipP {
public:
	ShipPPlayerA();
};
class ShipPPlayerB : public ShipP {
public:
	ShipPPlayerB();
};

class ShipM : public Ship {
public:
	ShipM() : Ship(SHIP_SIZE_M, SHIP_POINTS_M) {}
};

class ShipMPlayerA : public ShipM {
public:
	ShipMPlayerA();
};
class ShipMPlayerB : public ShipM {
public:
	ShipMPlayerB();
};

class ShipD : public Ship {
public:
	ShipD() : Ship(SHIP_SIZE_D, SHIP_POINTS_D) {}
};

class ShipDPlayerA : public ShipD {
public:
	ShipDPlayerA();
};
class ShipDPlayerB : public ShipD {
public:
	ShipDPlayerB();
};

// Use this function only with valid chars
Ship *charToShip(char c);

vector<Ship*> &deepCopyShipPointerVector(const vector<Ship*> &shipList);
int shootShip(Point pointHit, vector<Ship*> &shipList);