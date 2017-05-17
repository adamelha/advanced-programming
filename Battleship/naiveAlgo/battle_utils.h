#pragma once

typedef enum player { player_A = 0, player_B = 1 } player_is;
typedef enum squareType { NO_SQUARE = 0, A_SQUARE = 1, B_SQUARE = 2 } squareType_e;
int isBelongToBoard(const char c);