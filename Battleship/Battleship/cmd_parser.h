#pragma once

#include <string>

using namespace std;

enum cmdParser_t {CMD_PARSER_OK, CMD_PARSER_ERROR, CMD_PARSER_TOO_MANY_ARGS};

class CmdParser {
public:
	cmdParser_t parse();
	string getFilesPath();
	CmdParser(int argc, char **argv) : argc(argc), argv(argv) {}
private:
	string filesPath;
	int argc;
	char **argv;
};