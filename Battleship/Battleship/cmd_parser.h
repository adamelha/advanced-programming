#pragma once

#include <string>
#include "status.h"
#include "config.h"

using namespace std;

class CmdParser {
public:
	status_t parse();
	string getFilesPath();
	CmdParser(int argc, char **argv) : argc(argc), argv(argv), printActive(true), threadNumber(DEFAULT_THREAD_NUMBER){}
	bool isPrintActive() { return printActive; }
	int getThreadNumber() { return threadNumber; }
	bool printActive;
private:
	int threadNumber;
	string filesPath;
	int argc;
	char **argv;
};
