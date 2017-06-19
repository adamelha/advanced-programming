#pragma once
#include "config.h"
#include <string>
#include <vector>

using namespace std;

enum class ErrorPrintType
{
	NoPrint,
	CoutPrint,
	LogfilePrint
};
class ErrorClass
{
public:
	ErrorClass(size_t num_of_err_msgs);
	ErrorClass(size_t num_of_err_msgs, ErrorPrintType type);
	void addErrorMsg(size_t index, string msg);
	void printErrorMsg();
	void printSingleMsg(string msg);
	bool errorMsgsExist();
	~ErrorClass();
	size_t num_of_err_msgs;
	vector<string> errorMsgs;
	ErrorPrintType printType;
	string logFilePath;
};

