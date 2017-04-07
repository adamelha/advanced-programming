#pragma once

#include <string>
#include "status.h"

using namespace std;

class CmdParser {
public:
	status_t parse();
	string getFilesPath();
	CmdParser(int argc, char **argv) : argc(argc), argv(argv) {}
private:
	string filesPath;
	int argc;
	char **argv;
};
