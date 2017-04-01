#include "cmd_parser.h"
#include <iostream>

using namespace std;

//TODO: does not work when path from command line has spaces. Need fix?
cmdParser_t CmdParser::parse()
{
	cmdParser_t status = CMD_PARSER_OK;
	
	switch (this->argc)
	{
	case(1):
		this->filesPath = "";
		break;
		
	case(2):
		this->filesPath = argv[1];
		break;

	default:
		std::cout << "Too many arguments given.";
		status = CMD_PARSER_TOO_MANY_ARGS;
	}
	
	return status;
}

string CmdParser::getFilesPath()
{
	return this->filesPath;
}