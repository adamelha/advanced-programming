#include "display.h"
#include <stdlib.h>
#include "macros.h"
#include <Windows.h>
#include <algorithm>

Display::Display()
{
	
}

// Gets list of scores of a single round and prints them in a thread safe manner
void Display::displayTabel(std::vector<PlayerScore> scoreArrayForRound)
{
	// Calculate winning percentage for each player, and give name and rank
	// Does everything on local copy
	for (size_t i = 0; i < scoreArrayForRound.size(); i++)
	{
		scoreArrayForRound[i].percentage = ((double)scoreArrayForRound[i].wins / ((double)scoreArrayForRound[i].losses + (double)scoreArrayForRound[i].wins)) * 100;
		scoreArrayForRound[i].name = teamNames[i];
	}
	
	// Sort by winning percentage in descending order
	std::sort(scoreArrayForRound.begin(), scoreArrayForRound.end(), greater<PlayerScore>());
	
	// Update rank
	for (size_t i = 0; i < scoreArrayForRound.size(); i++)
	{
		scoreArrayForRound[i].rank = i + 1;
	}

	printMutex.lock();

	cout << "#" << "\t";
	cout << "Team Name" << "\t";
	cout << "Wins" << "\t";
	cout << "Losses" << "\t";
	cout << "%" << "\t";
	cout << "Pts For" << "\t";
	cout << "Pts Against" << "\t\n\n";

	for (size_t i = 0; i < scoreArrayForRound.size(); i++)
	{
		displayRow(scoreArrayForRound[i]);
	}

	printMutex.unlock();

}
void Display::hideCursor()
{
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &info);

}

// Print single row. NOT thread safe, called only by displayTabel
void Display::displayRow(PlayerScore row)
{
	double pct;

	// Must print team name
	cout << row.rank << "\t";
	cout << row.name << "\t";
	cout << row.wins << "\t";
	cout << row.losses << "\t";
	cout << row.percentage << "\t";
	cout << row.totalPointsFor << "\t";
	cout << row.totalPointsAgainst << "\t\n";
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
	return Point(SBInfo.dwCursorPosition.X, SBInfo.dwCursorPosition.Y, 0);

}


// if print is active, sleeps for cmd.delay milliseconds and then updates
void Display::updateSquare(int y, int x, char updatedChar)
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