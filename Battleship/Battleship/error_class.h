#pragma once

#include <string>
#include <vector>
using namespace std;

class ErrorClass
{
public:
	ErrorClass(size_t num_of_err_msgs);
	void addErrorMsg(size_t index, string msg);
	void printErrorMsg();
	bool errorMsgsExist();
	~ErrorClass();
	size_t num_of_err_msgs;
	vector<string> errorMsgs;
};

