#pragma once
#include <Windows.h>
#include "board.h"
#include "cmd_parser.h"
#include "point.h"
using namespace std;

#define BOARD_DISPLAY_OFFSET_X	0
#define BOARD_DISPLAY_OFFSET_Y	10

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

#define TEXT_COLOR_DEFAULT			WHITE
#define TEXT_COLOR_PLAYER_A			GREEN
#define TEXT_COLOR_PLAYER_B			YELLOW
#define TEXT_COLOR_BOMBED			RED


class Display
{
public:
	Display(CmdParser cmd);
	void gotoxy(int x, int y);
	void hideCursor();
	void setTextColor(int color);
	void printBoard(const Board &board);
	void updateSquare(int x, int y, char updatedChar);
	Point getCursorPosition();

private:
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	HANDLE hConsole;
	bool printBoardActive;
	uintmax_t delay;
	
	//POINT currentCursorLocation;
	Point currentCursorLocation;
};

