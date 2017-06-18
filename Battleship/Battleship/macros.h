#pragma once

#include <iostream>
#include <mutex>

#define DEBUG

// Mutex for debug printing - debug and Display
extern std::mutex printMutex;


#ifdef DEBUG
#include <stdio.h>
#include <thread>

// Print DEBUG_PRINT and INFO_PRINT
#define PRIORITY_DEBUG	1
// Print only INFO_PRINT
#define PRIORITY_INFO	2

#define BUILD_DEBUG_PRIORITY	PRIORITY_INFO


#define DEBUG_PRINT(format, ...)	PRINT_LOG(PRIORITY_DEBUG, format, ##__VA_ARGS__)

#define INFO_PRINT(format, ...)		PRINT_LOG(PRIORITY_INFO, format, ##__VA_ARGS__)

#define PRINT_LOG(priority, format, ...) \
do { \
if(priority >= BUILD_DEBUG_PRIORITY) { \
printMutex.lock(); \
std::cout << "[Thread " << std::this_thread::get_id() << "]"; \
printf(format, ##__VA_ARGS__); \
printMutex.unlock(); \
} \
} while(0)

#else
#define DEBUG_PRINT(format, ...)
#define INFO_PRINT(format, ...)
#endif // DEBUG

// Define 1 - random seed from clock ticks.
// Define 0 - random seed 0.
// Better to define 0 when debugging for easier debg
#define RANDOM_TRUE		0

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
