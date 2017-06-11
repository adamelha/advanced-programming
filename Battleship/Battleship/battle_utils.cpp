#include "battle_utils.h"


int isBelongToBoard(const char c)
{
	if (c == 'B' || c == 'P' || c == 'M' || c == 'D')         //A Square
		return A_SQUARE;
	else if (c == 'b' || c == 'p' || c == 'm' || c == 'd')     // B sqare
		return B_SQUARE;
	else {
		return NO_SQUARE;
	}
}

