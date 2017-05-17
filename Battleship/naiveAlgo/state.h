#pragma once
#include "cmd_parser.h"
#include "file_parser.h"
#include "board.h"
#include "status.h"
//#include "battle.h"
#include "macros.h"

class State
{
public:
	Board boardKnown;
	Point prevPoint;
	virtual Point getNextPotentialPoint() = 0;
	virtual  Point * potentialLegalMoves() = 0;
};