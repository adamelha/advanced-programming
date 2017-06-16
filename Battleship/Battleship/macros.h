#pragma once

#include <iostream>
#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#include <mutex>
#include <thread>

// Mutex for debug printing
extern std::mutex debugPrintMutex;
#define DEBUG_PRINT(format, ...) \
debugPrintMutex.lock(); \
std::cout << "[Thread " << std::this_thread::get_id() << "]"; \
printf(format, ##__VA_ARGS__); \
debugPrintMutex.unlock()

#else
#define DEBUG_PRINT(format, ...)
#endif // DEBUG


#define ARRAY_LENGTH(arr)	(sizeof(arr) / sizeof((arr[0])))

/*
This macro accepts only a string (one arg).
For format string must snprintf before using this macro.
*/
#define ERROR_PRINT(message, returnValue) \
		std::cout << "Error: " << message << "\n"; \
		return returnValue;

#define BOARD_SIZE						10

#define PLAYER_A	0
#define PLAYER_B	1

#define IS_PLAYER_A(c)	(isupper(c))

#define SQUARE_BOMBED_MISS_SYMBOL		'@'
#define SQUARE_BOMBED_HIT_SYMBOL		'*'

// This returned by IBattleshipGameAlgo.attack() if no moves left
#define ALGO_OUT_OF_MOVES		-1

#define EXIT_SUCCESS	0
#define EXIT_FAIL		-1

// IMPORTANT: rows, cols, and depth must be defined in scope where this macro is used!
#define isPointOnBoard(x , y, z)  ( x < rows && x > -1 && y < cols && y > -1 && z < depth && z > -1)
