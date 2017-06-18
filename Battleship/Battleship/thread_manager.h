#pragma once

#include "cmd_parser.h"
#include "file_parser.h"
#include "board.h"
#include "status.h"
#include "IBattleshipGameAlgo.h"
#include "battle.h"
#include "battle_utils.h"
#include <iostream>
#include <fstream>
#include <thread>
#include "macros.h"
#include <stdlib.h>
#include <Windows.h>
#include <atomic>
#include <vector>
#include <chrono>
#include "display.h"

#define NOT_ENOUGH_DLLS_MSG			"Missing algorithm (dll) files looking in path: " + path + " (needs at least two)"
#define NOT_ENOUGH_DLLS_IDX			0
#define NUM_OF_DLL_ERR_MSGS			1

typedef IBattleshipGameAlgo *(*GetAlgoFuncType)();

struct notEnoughDlls {};
struct gameInfo
{
public:
	gameInfo(int firstPlayer_ , int secondPlayer_ , int boardIndex_) : firstPlayer(firstPlayer_) , secondPlayer(secondPlayer_) , boardIndex(boardIndex_){}
	int firstPlayer;
	int secondPlayer;
	int boardIndex;
};

class ThreadManager {
public:
	ThreadManager(const string& _path, const vector<Board>& _boards, int numberOfThreads);
	status_t run();
	
private:
	std::vector<HINSTANCE> dllList;
	std::vector<std::string> teamNameList;
	//std::vector<IBattleshipGameAlgo *> playersList;
	const string& path;
	//const Board& board;
	const vector<Board>& boards;
	int numOfSquareA = 0, numOfSquareB = 0;
	void threadGameFunc();

	// Use  std::atomic_fetch_add
	std::atomic<int> atomicCounter = -1;
	std::vector<thread> threadList;
	int numberOfThreads;
	bool loadDllFiles();
	
	GetAlgoFuncType getAlgoFunc;
	//std::vector< std::unique_ptr<GetAlgoFuncType> > algoFuncInstance1;
	std::vector< gameInfo > listOfGames;
	void creatListOfGames();
	void initial_numberOfRounds_numberOfPlayers_scoreTabel_playerRound();

	// This is 2d tabel: scoreTabel[round][player]
	//PlayerScore **scoreTabel;

	// Array of PlayerScore vectors - each member of the array is a vector of the scores of the players for a round
	// scoreTabel[round][player] - the PlayerScore for round 'round' for player 'player'
	std::vector<std::vector<PlayerScore>> scoreTabel;
	// For each dll tells round number
	std::atomic<int> *playerRound;
	int numberOfRounds, numberOfPlayers;
	Display display;
};
