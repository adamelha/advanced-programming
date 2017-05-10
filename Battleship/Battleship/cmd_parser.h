#pragma once

#include <string>
#include "status.h"

#define DEFAULT_DELAY_IN_MS	500

using namespace std;

class CmdParser {
public:
	status_t parse();
	string getFilesPath();
	CmdParser(int argc, char **argv) : argc(argc), argv(argv), printActive(true), delay(DEFAULT_DELAY_IN_MS){}
	bool isPrintActive() { return printActive; }
	uintmax_t getDelay() { return delay; }
	bool printActive;
private:
	
	uintmax_t delay;
	string filesPath;
	int argc;
	char **argv;
};
