#include "cmd_parser.h"
#include <iostream>
#include "macros.h"
#include <algorithm>
#include <inttypes.h>
#include <vector>
using namespace std;

status_t CmdParser::parse()
{
	status_t status = STATUS_OK;
	this->filesPath = "";
	//ptrdiff_t idx;      Unused variable
	vector<int> usedIndexList;
	vector<string> cmdList;

	// command line to string list (without argv[0])
	for (size_t i = 1; i < argc; i++)
	{
		cmdList.push_back(string(argv[i]));
	}

	// Parse quiet
	if (std::find(cmdList.begin(), cmdList.end(), "-quiet") != cmdList.end())
	{
		usedIndexList.push_back(std::find(cmdList.begin(), cmdList.end(), "-quiet") - cmdList.begin());
		DEBUG_PRINT("Found quiet param\n");
		this->printActive = false;
	}

	// Parse delay
	if (std::find(cmdList.begin(), cmdList.end(), "-delay") != cmdList.end())
	{
		int delayIdx = std::find(cmdList.begin(), cmdList.end(), "-delay") - cmdList.begin();
		usedIndexList.push_back(delayIdx);
		DEBUG_PRINT("Found delay param\n");
		
		// If -delay is last in cmd list (no provided value)
		if (delayIdx == cmdList.size() - 1) {
			ERROR_PRINT("Must provide -delay with value", STATUS_ERROR);
		}
		else {
			int num;
			try
			{
				num = std::stoi(cmdList[delayIdx + 1]);
			}
			catch (...)
			{
				ERROR_PRINT("Unable to parse delay provided, make sure you provide an integer", STATUS_ERROR);
			}
			usedIndexList.push_back(delayIdx + 1);
			this->delay = num;
		}
	}

	// Parse file path
	for (size_t i = 0; i < cmdList.size(); i++)
	{
		if (std::find(usedIndexList.begin(), usedIndexList.end(), i) != usedIndexList.end())
		{
			continue;
		}
		else {
			this->filesPath = cmdList[i];
			break;
		}
	}

	return status;
}

string CmdParser::getFilesPath()
{
	return this->filesPath;
}