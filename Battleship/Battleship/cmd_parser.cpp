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
	vector<int> usedIndexList;
	vector<string> cmdList;

	// command line to string list (without argv[0])
	for (size_t i = 1; i < argc; i++)
	{
		cmdList.push_back(string(argv[i]));
	}

	// Parse threads
	if (std::find(cmdList.begin(), cmdList.end(), "-threads") != cmdList.end())
	{
		int threadIdx = std::find(cmdList.begin(), cmdList.end(), "-threads") - cmdList.begin();
		usedIndexList.push_back(threadIdx);
		DEBUG_PRINT("Found threads param\n");
		
		// If -delay is last in cmd list (no provided value)
		if (threadIdx == cmdList.size() - 1) {
			ERROR_PRINT("Must provide -threads with value", STATUS_ERROR);
		}
		else {
			int num;
			try
			{
				num = std::stoi(cmdList[threadIdx + 1]);
			}
			catch (...)
			{
				ERROR_PRINT("Unable to parse threads provided, make sure you provide an integer", STATUS_ERROR);
			}
			usedIndexList.push_back(threadIdx + 1);
			this->threadNumber = num;
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