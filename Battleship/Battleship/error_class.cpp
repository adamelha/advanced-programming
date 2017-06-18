#include "error_class.h"
#include <iostream>

ErrorClass::ErrorClass(size_t num_of_err_msgs)
{
	this->num_of_err_msgs = num_of_err_msgs;
	this->errorMsgs.resize(num_of_err_msgs);

	// Nullify strings
	for (size_t i = 0; i < errorMsgs.size(); i++)
	{
		errorMsgs[i] = "";
	}

}

void ErrorClass::addErrorMsg(size_t index, string msg)
{
	if (errorMsgs[index] == "") {
		errorMsgs[index] = msg;
	}
}

void ErrorClass::printErrorMsg()
{
	size_t i;

	// Print all error messages
	for (i = 0; i < num_of_err_msgs; i++)
	{
		if (errorMsgs[i] != "")
		{
			cout << errorMsgs[i];
		}
	}


}

bool ErrorClass::errorMsgsExist()
{
	size_t i;

	for (i = 0; i < num_of_err_msgs; i++)
	{
		if (errorMsgs[i] != "") {
			return true;
		}
	}

	return false;
}
ErrorClass::~ErrorClass()
{
	
}
