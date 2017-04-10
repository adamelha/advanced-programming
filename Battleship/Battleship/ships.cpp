#include "ships.h"
#include "macros.h"
ShipBPlayerA::ShipBPlayerA() { ShipB(); charSymbol = 'B'; player = 'A'; msgWrongSizeIdx = 0; }
ShipBPlayerB::ShipBPlayerB() { ShipB(); charSymbol = 'b'; player = 'B'; msgWrongSizeIdx = 4; }

ShipPPlayerA::ShipPPlayerA() { ShipP(); charSymbol = 'P'; player = 'A'; msgWrongSizeIdx = 1; }
ShipPPlayerB::ShipPPlayerB() { ShipP(); charSymbol = 'p'; player = 'B'; msgWrongSizeIdx = 5; }

ShipMPlayerA::ShipMPlayerA() { ShipM(); charSymbol = 'M'; player = 'A'; msgWrongSizeIdx = 2; }
ShipMPlayerB::ShipMPlayerB() { ShipM(); charSymbol = 'm'; player = 'B'; msgWrongSizeIdx = 6; }

ShipDPlayerA::ShipDPlayerA() { ShipD(); charSymbol = 'D'; player = 'A'; msgWrongSizeIdx = 3; }
ShipDPlayerB::ShipDPlayerB() { ShipD(); charSymbol = 'd'; player = 'B'; msgWrongSizeIdx = 7; }

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