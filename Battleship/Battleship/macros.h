#pragma once

#define DEBUG	1

#define DEBUG_PRINT(format, ...) do {\
		if(DEBUG) { \
		printf(format, ##__VA_ARGS__);\
		} \
} while(0)

#define ARRAY_LENGTH(arr)	(sizeof(arr) / sizeof((arr[0])))

/*
This macro accepts only a string (one arg).
For format string must snprintf before using this macro.
*/
#define ERROR_PRINT(message, returnValue) \
		std::cout << "Error: " << message << "\n"; \
		return returnValue;

#define BOARD_SIZE						10
