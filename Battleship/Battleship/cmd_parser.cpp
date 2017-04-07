#include "cmd_parser.h"
#include <iostream>
#include "macros.h"
using namespace std;

//TODO: does not work when path from command line has spaces. Need fix?
status_t CmdParser::parse()
{
	status_t status = STATUS_OK;
	
	switch (this->argc)
	{
	case(1):
		this->filesPath = "";
		break;
		
	case(2):
		this->filesPath = argv[1];
		break;

	default:
		ERROR_PRINT("Too many arguments given.", STATUS_ERROR);
	}
	
	return status;
}

string CmdParser::getFilesPath()
{
	return this->filesPath;
}