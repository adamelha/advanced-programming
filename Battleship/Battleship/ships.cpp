#include "ships.h"
#include "macros.h"

// Copy constructor
Ship::Ship(const Ship &ship)
{
	operator=(ship);
}

Ship& Ship::operator = (const Ship& ship)
{
	this->charSymbol = ship.charSymbol;
	this->msgAdjacentShips = msgAdjacentShips;
	this->msgAdjacentShipsIdx = msgAdjacentShipsIdx;
	this->msgTooFewShips = msgTooFewShips;
	this->msgTooFewShipsIdx = msgTooFewShipsIdx;
	this->msgTooManyShips = msgTooManyShips;
	this->msgTooManyShipsIdx = msgTooManyShipsIdx;
	this->msgWrongSize = msgWrongSize;
	this->msgWrongSizeIdx = msgWrongSizeIdx;
	this->player = this->player;
	this->pointList = new Point[size];
	this->size = size;
	this->pointsWorth = pointsWorth;

	return *this;
}
void Ship::initErrorStrings() 
{
	msgWrongSize = string("Wrong size or shape for ship ") + charSymbol + string(" for player ") + player + '\n';
	msgTooManyShips = "Too many ships for player " + player + '\n';
	msgTooFewShips = "Too few ships for player " + player + '\n';
}

int Ship::getPointsWorth() 
{
	return pointsWorth;
}


ShipBPlayerA::ShipBPlayerA() : ShipB() { charSymbol = 'B'; player = 'A'; msgWrongSizeIdx = 0; initErrorStrings();}
ShipBPlayerB::ShipBPlayerB() : ShipB() { charSymbol = 'b'; player = 'B'; msgWrongSizeIdx = 4; initErrorStrings();}

ShipPPlayerA::ShipPPlayerA() { ShipP(); charSymbol = 'P'; player = 'A'; msgWrongSizeIdx = 1; initErrorStrings();}
ShipPPlayerB::ShipPPlayerB() { ShipP(); charSymbol = 'p'; player = 'B'; msgWrongSizeIdx = 5; initErrorStrings();}

ShipMPlayerA::ShipMPlayerA() { ShipM(); charSymbol = 'M'; player = 'A'; msgWrongSizeIdx = 2; initErrorStrings();}
ShipMPlayerB::ShipMPlayerB() { ShipM(); charSymbol = 'm'; player = 'B'; msgWrongSizeIdx = 6; initErrorStrings();}

ShipDPlayerA::ShipDPlayerA() { ShipD(); charSymbol = 'D'; player = 'A'; msgWrongSizeIdx = 3; initErrorStrings();}
ShipDPlayerB::ShipDPlayerB() { ShipD(); charSymbol = 'd'; player = 'B'; msgWrongSizeIdx = 7; initErrorStrings();}

Ship *charToShip(char c)
{
	switch (c)
	{
	case 'B':
		return new ShipBPlayerA();
	case 'b':
		return new ShipBPlayerB();
	case 'P':
		return new ShipPPlayerA();
	case 'p':
		return new ShipPPlayerB();
	case 'M':
		return new ShipMPlayerA();
	case 'm':
		return new ShipMPlayerB();
	case 'D':
		return new ShipDPlayerA();
	case 'd':
		return new ShipDPlayerB();
	default:
		// If we got here this means there is a bug in our program.
		DEBUG_PRINT("charToShip() got an incompatible char. We likely have a bug\n");
		return nullptr;
	}
}