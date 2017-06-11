#include "display.h"
#include <stdlib.h>
#include "macros.h"
#include <Windows.h>

/*

Display::Display(CmdParser cmd)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printBoardActive = cmd.printActive;
	delay = cmd.getDelay();
}

void Display::hideCursor()
{
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);
	
}

// Sets text color to color. Must remember to restore to default color after colored print done.
void Display::setTextColor(int color)
{
	SetConsoleTextAttribute(hConsole, color);
}

// Goes to coordinates given
void Display::gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(hConsole, c);
}

Point Display::getCursorPosition()
{
	GetConsoleScreenBufferInfo(hConsole, &SBInfo);
	return Point(SBInfo.dwCursorPosition.X, SBInfo.dwCursorPosition.Y);
	
}

// Prints board in colors if 

void Display::printBoard(const Board &boardObj)
{
	currentCursorLocation = getCursorPosition();

	if (printBoardActive)
	{
		for (size_t x = 0; x < BOARD_SIZE; x++)
		{
			for (size_t y = 0; y < BOARD_SIZE; y++)
			{
				gotoxy(BOARD_DISPLAY_OFFSET_X + x, BOARD_DISPLAY_OFFSET_Y + y);
				if (IS_PLAYER_A(boardObj.board[y][x])) {
					setTextColor(TEXT_COLOR_PLAYER_A);
				}
				else {
					setTextColor(TEXT_COLOR_PLAYER_B);
				}
				cout << boardObj.board[y][x];
			}

		}
	}

	//Restore
	gotoxy(currentCursorLocation.row, currentCursorLocation.col);
	setTextColor(TEXT_COLOR_DEFAULT);
}

// if print is active, sleeps for cmd.delay milliseconds and then updates
void Display::updateSquare( int y, int x, char updatedChar)
{
	if (x < 0)
		return;
	if (printBoardActive)
	{
		if (x < 0)
		{
			return;
		}
		//GetCursorPos(&currentCursorLocation);
		//ScreenToClient(hConsole, &currentCursorLocation);
		currentCursorLocation = getCursorPosition();
		Sleep(delay);
		gotoxy(BOARD_DISPLAY_OFFSET_X + x, BOARD_DISPLAY_OFFSET_Y + y);

		

		if (updatedChar == SQUARE_BOMBED_MISS_SYMBOL || updatedChar == SQUARE_BOMBED_HIT_SYMBOL)
		{
			setTextColor(TEXT_COLOR_BOMBED);
			
		}
		else if (IS_PLAYER_A(updatedChar)) {
			setTextColor(TEXT_COLOR_PLAYER_A);
		}
		else {
			setTextColor(TEXT_COLOR_PLAYER_B);
		}

		cout << updatedChar;

		// Restore
		//gotoxy(0, 30);
		gotoxy(currentCursorLocation.row, currentCursorLocation.col);
		setTextColor(TEXT_COLOR_DEFAULT);
	}
	
	
}
*/