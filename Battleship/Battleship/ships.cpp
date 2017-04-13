#include "ships.h"
#include "macros.h"
#include <vector>

vector<Ship*> &deepCopyShipPointerVector(const vector<Ship*> &shipList)
{
	vector<Ship*> *retShipList = new vector<Ship*>();

	for (size_t i = 0; i < shipList.size(); i++)
	{
		(*retShipList).push_back(new Ship(*shipList[i]));
	}

	return *retShipList;
}

// finds the coordinate of the ship part hit, 'removes' it from ship pointList by making that point invalid, decreasing ship.activePointsLeft, and returning the score received if ship sinks.
// return number of points gained
int shootShip(Point pointHit, vector<Ship*> &shipList)
{
	int pointsGained = 0;
	for (size_t shipIdx = 0; shipIdx < shipList.size(); shipIdx++)
	{
		for (size_t pointIdx = 0; pointIdx < shipList[shipIdx]->size; pointIdx++)
		{
			// Found point to remove
			if (pointHit == shipList[shipIdx]->pointList[pointIdx])
			{
				// Remove point from ship
				shipList[shipIdx]->pointList[pointIdx].invalidatePoint();
				
				// If no active points left in ship - remove from ship list
				if (--(shipList[shipIdx]->activePointsLeft) == 0)
				{
					pointsGained = shipList[shipIdx]->getPointsWorth();
					delete shipList[shipIdx];
					shipList.erase(shipList.begin() + shipIdx);

					return pointsGained;
				}
				
			}
		}
	}

	return pointsGained;
}
// Copy constructor
Ship::Ship(const Ship &ship)
{
	*this = ship;
}

Ship& Ship::operator = (const Ship& ship)
{
	this->charSymbol = ship.charSymbol;
	this->msgAdjacentShips = ship.msgAdjacentShips;
	this->msgAdjacentShipsIdx = ship.msgAdjacentShipsIdx;
	this->msgTooFewShips = ship.msgTooFewShips;
	this->msgTooFewShipsIdx = ship.msgTooFewShipsIdx;
	this->msgTooManyShips = ship.msgTooManyShips;
	this->msgTooManyShipsIdx = ship.msgTooManyShipsIdx;
	this->msgWrongSize = ship.msgWrongSize;
	this->msgWrongSizeIdx = ship.msgWrongSizeIdx;
	this->player = ship.player;

	this->pointList = new Point[ship.size];
	this->size = ship.size;
	for (size_t i = 0; i < ship.size; i++)
	{
		this->pointList[i] = ship.pointList[i];
	}

	this->pointsWorth = ship.pointsWorth;
	this->activePointsLeft = ship.activePointsLeft;
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