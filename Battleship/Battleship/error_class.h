#pragma once

#include <string>

using namespace std;

class ErrorClass
{
public:
	ErrorClass(size_t num_of_err_msgs);
	void printErrorMsg();
	~ErrorClass();
	size_t num_of_err_msgs;
	string **errorMsgs;
};

