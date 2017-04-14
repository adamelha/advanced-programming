#pragma once

//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(format, ...)	printf(format, ##__VA_ARGS__)
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


#define EXIT_SUCCESS	0
#define EXIT_FAIL		-1
