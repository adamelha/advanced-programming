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

#define CANNOT_LOAD_DLL_MSG			"Cannot load dll: " + fullFileName + "\n"
#define CANNOT_LOAD_DLL_IDX			0

#define ALGO_INIT_FAIL_MSG			"Algorithm initialization failed for dll: " + fullFileName + "\n"
#define ALGO_INIT_FAIL_IDX			1


#define ALGO_INIT_FAILED			"Cannot load dll: " << fullFileName << endl

#define NUM_OF_DLL_ERR_MSGS			2

#define TEMP_MAX_DLLS	10


class ThreadManager {
public:
	ThreadManager(const string& _path, const Board& _board, int numberOfThreads);
	~ThreadManager() { delete[] scoreTabel; }
	status_t run();
	
private:
	std::vector<HINSTANCE> dllList;
	//std::vector<IBattleshipGameAlgo *> playersList;
	const string& path;
	const Board& board;
	int numOfSquareA = 0, numOfSquareB = 0;
	void threadGameFunc();

	// Use  std::atomic_fetch_add
	std::atomic<int> atomicCounter = -1;
	std::vector<thread> threadList;
	int numberOfThreads;
	bool loadDllFiles();
	typedef IBattleshipGameAlgo *(*GetAlgoFuncType)();
	GetAlgoFuncType getAlgoFunc;
	//std::vector< std::unique_ptr<GetAlgoFuncType> > algoFuncInstance1;
	std::vector< pair<int, int> > listOfGames;
	void creatListOfGames();

	// This is 2d tabel: scoreTabel[round][player]
	PlayerScore **scoreTabel;

	// For each dll tells round number
	std::atomic<int> *playerRound;
	int numberOfRounds, numberOfPlayers;
	Display display;
};
