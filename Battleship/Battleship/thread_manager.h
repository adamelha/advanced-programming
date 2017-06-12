#pragma once

#include "cmd_parser.h"
#include "file_parser.h"
#include "board.h"
#include "status.h"
#include "IBattleshipGameAlgo.h"
#include <iostream>
#include <fstream>
#include <thread>
#include "macros.h"
#include <stdlib.h>
#include <Windows.h>
#include "board.h"
#include "status.h"
#include <atomic>
#include <vector>



#define CANNOT_LOAD_DLL_MSG			"Cannot load dll: " + fullFileName + "\n"
#define CANNOT_LOAD_DLL_IDX			0

#define ALGO_INIT_FAIL_MSG			"Algorithm initialization failed for dll: " + fullFileName + "\n"
#define ALGO_INIT_FAIL_IDX			1


#define ALGO_INIT_FAILED			"Cannot load dll: " << fullFileName << endl

#define NUM_OF_DLL_ERR_MSGS			2


class ThreadManager {
public:
	ThreadManager(const string& _path, const Board& _board, int numberOfThreads);
	status_t run();
	
private:
	std::vector<HINSTANCE> dllList;
	//std::vector<IBattleshipGameAlgo *> playersList;
	const string& path;
	const Board& board;
	int numOfSquareA = 0, numOfSquareB = 0;
	void threadGameFunc();

	// Use  std::atomic_fetch_add
	std::atomic<int> atomicCounter;
	std::vector<thread> threadList;
	int numberOfThreads;
	bool loadDllFiles();
};
