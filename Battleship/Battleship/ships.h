#pragma once
#include "point.h"
#include <string>
using namespace std;

#define SHIP_SIZE_B				1
#define SHIP_SIZE_P				2
#define SHIP_SIZE_M				3
#define SHIP_SIZE_D				4

#define NUM_OF_DIFFERENT_SHIPS	4
#define NUM_OF_PLAYERS			2

#define MSG_MANY_SHIPS_A		"Too many ships for player A\n"
#define MSG_MANY_SHIPS_A_IDX	(NUM_OF_DIFFERENT_SHIPS * NUM_OF_PLAYERS)
#define MSG_FEW_SHIPS_A			"Too few ships for player A"
#define MSG_FEW_SHIPS_A_IDX		(MSG_MANY_SHIPS_A_IDX + 1)
#define MSG_MANY_SHIPS_B		"Too many ships for player B\n"
#define MSG_MANY_SHIPS_B_IDX	(MSG_FEW_SHIPS_A_IDX + 1)
#define MSG_FEW_SHIPS_B			"Too few ships for player B"
#define MSG_FEW_SHIPS_B_IDX		(MSG_MANY_SHIPS_B_IDX + 1)

#define MSG_ADJACENT_SHIPS_IDX	(MSG_FEW_SHIPS_B_IDX + 1)
#define NUMBER_OF_BOARD_MSGS	(MSG_ADJACENT_SHIPS_IDX + 1)


class Ship {
public:
	Ship(size_t sizeOfShip) : pointList(new Point[sizeOfShip]),
		msgWrongSize(string("Wrong size or shape for ship ") + charSymbol + string("for player") + player ),
		msgTooManyShips("Too many ships for player " + player + '\n'),
		msgTooFewShips("Too few ships for player " + player + '\n'),
		msgAdjacentShips("Adjacent Ships on Board\n") {}
	char player;
	char charSymbol;
	size_t size;
	Point *pointList;
	size_t msgWrongSizeIdx, msgTooManyShipsIdx, msgTooFewShipsIdx, msgAdjacentShipsIdx;
	string msgWrongSize;
	string msgTooManyShips;
	string msgTooFewShips;
	string msgAdjacentShips;

	~Ship() { delete[] pointList; }
};

class ShipB : public Ship {
public:
	ShipB() : Ship(SHIP_SIZE_B) {}
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
	ShipP() : Ship(SHIP_SIZE_P) {}
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
	ShipM() : Ship(SHIP_SIZE_M) {}
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
	ShipD() : Ship(SHIP_SIZE_D) {}
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
