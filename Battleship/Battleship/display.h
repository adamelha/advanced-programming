#pragma once
#include <Windows.h>
#include "board.h"
#include "cmd_parser.h"
#include "point.h"
#include <mutex>

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

struct PlayerScore
{
	int totalPointsFor, totalPointsAgainst, wins, losses;
	string name;
};

class Display
{
public:	
	Display();

	void gotoxy(int x, int y);
	void hideCursor();
	void setTextColor(int color);
	void updateSquare(int x, int y, char updatedChar);
	Point getCursorPosition();
	void displayTabel(PlayerScore *scoreArrayForRound, int numberOfPlayers);

private:
	void displayRow(PlayerScore row);
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	CONSOLE_CURSOR_INFO info;
	HANDLE hConsole;
	bool printBoardActive;
	uintmax_t delay;

	//POINT currentCursorLocation;
	Point currentCursorLocation;
};

