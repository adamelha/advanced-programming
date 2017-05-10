#include "display.h"
#include <stdlib.h>
#include "macros.h"


Display::Display(CmdParser cmd)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printBoardActive = cmd.printActive;
}

void Display::hideCursor()
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = TRUE;
	SetConsoleCursorInfo(hConsole, &info);
}

void Display::setTextColor(int color)
{
	SetConsoleTextAttribute(hConsole, color);
}

void Display::gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(hConsole, c);
}

void Display::printBoard(const Board &boardObj)
{
	if (printBoardActive)
	{
		for (size_t x = 0; x < BOARD_SIZE; x++)
		{
			for (size_t y = 0; y < BOARD_SIZE; y++)
			{
				gotoxy(BOARD_DISPLAY_OFFSET_X + x, BOARD_DISPLAY_OFFSET_Y + y);
				if (IS_PLAYER_A(boardObj.board[x][y])) {
					setTextColor(TEXT_COLOR_PLAYER_A);
				}
				else {
					setTextColor(TEXT_COLOR_PLAYER_B);
				}
				cout << boardObj.board[x][y];
			}

		}
	}
	setTextColor(TEXT_COLOR_DEFAULT);
}