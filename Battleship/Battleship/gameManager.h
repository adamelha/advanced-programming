#pragma once
#pragma once

#include "IBattleshipGameAlgo.h"
#include "displayGame.h"
#include <list>
#include <windows.h>
#include <algorithm>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <string>
#include <direct.h>
#include <sstream>
#include <vector>
#include <map> 

using namespace std;

#define BOARD_EDGE_LENGTH 10
#define NUMBER_OF_PLAYERS 2
#define FIRST_PLAYER_INDEX 0
#define SECOND_PLAYER_INDEX 1
#define NUMBER_OF_SHIP_TYPES 4
#define VALID_NUMBER_OF_SHIPS 5
#define INITIAL_SHIPS_NUMBER 5
#define BUFFER_LENGTH 4096
#define ZERO_LENGTH 0

class gameManager {

	std::vector<IBattleshipGameAlgo *> players;
	std::vector<HINSTANCE> dllLibs;
	int nextMovePlayerIndex;
	list<pair<int, int>> fleet[NUMBER_OF_PLAYERS][INITIAL_SHIPS_NUMBER];
	int shipsValues[NUMBER_OF_PLAYERS][INITIAL_SHIPS_NUMBER];
	int numberOfShipsLeft[NUMBER_OF_PLAYERS];
	int scores[NUMBER_OF_PLAYERS];
	bool playerHasMoves[NUMBER_OF_PLAYERS];

public:

	gameManager();
	~gameManager();

	//functions that is used in the main function
	bool loadBoard(char** board, const string& path);
	bool isBoardValid(char** board);
	bool dllsExist(const string& path);
	bool executeGame(const char** board, const char** playerABoard, const char** playerBBoard, const string& path, int delay, bool quiet);
	void setPlayersBoards(const char** board, char** playerABoard, char** playerBBoard);
	void generatePath(int argc, char *argv[], string& path, int* delay, bool* quiet);
	char** allocateBoard();
	void destroyBoards(char** board, char** playerABoard, char** playerBBoard);

private:

	//executeGame aid functions
	bool loadAndInitDllFilesAndSetBoards(const string& path, const char** playerABoard, const char** playerBBoard);
	void setFleets(const char** board);
	void addShipToFleet(int i, int j, char shipType, int playerIndex);
	int getShipValue(char character);
	AttackResult executeAttack(pair<int, int> square);

	//isBoardValid aid functions
	bool isThereAdjacentShipsOnBoard(char** board);
	void getPlayerAndExpectedLength(char shipType, int *player, int *expectedLength);
	bool validateSizeAndShapeOfAShip(char** board, bool boolBoard[BOARD_EDGE_LENGTH][BOARD_EDGE_LENGTH], int i, int j, int expectedLength);
	bool shipSetHorizontally(char** board, bool boolBoard[BOARD_EDGE_LENGTH][BOARD_EDGE_LENGTH], int i, int j, int expectedLength);
	bool shipSetVertically(char** board, bool boolBoard[BOARD_EDGE_LENGTH][BOARD_EDGE_LENGTH], int i, int j, int expectedLength);
};
