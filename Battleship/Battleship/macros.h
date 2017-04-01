#pragma once

#define DEBUG	0

#define DEBUG_PRINT(format, ...) do {\
		if(DEBUG) { \
		printf(format, ##__VA_ARGS__);\
		} \
} while(0)

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr[0])))