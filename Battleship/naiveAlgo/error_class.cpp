#include "error_class.h"
#include <iostream>

ErrorClass::ErrorClass(size_t num_of_err_msgs)
{
	this->num_of_err_msgs = num_of_err_msgs;
	this->errorMsgs = new string*[num_of_err_msgs];
	memset(this->errorMsgs, 0, this->num_of_err_msgs * sizeof(string*));
}

void ErrorClass::addErrorMsg(size_t index, string msg)
{
	if (errorMsgs[index] == nullptr) {
		errorMsgs[index] = new string(msg);
	}
}

void ErrorClass::printErrorMsg()
{
	size_t i;

	// Print all error messages
	for (i = 0; i < num_of_err_msgs; i++)
	{
		if (errorMsgs[i] != nullptr)
		{
			cout << *errorMsgs[i];
		}
	}


}

bool ErrorClass::errorMsgsExist()
{
	size_t i;

	// Print all error messages
	for (i = 0; i < num_of_err_msgs; i++)
	{
		if (errorMsgs[i] != nullptr) {
			return true;
		}
	}

	return false;
}
ErrorClass::~ErrorClass()
{
	size_t i;

	// Free allocated msg strings
	for (i = 0; i < num_of_err_msgs; i++)
	{
		if (errorMsgs[i] != NULL)
		{
			delete errorMsgs[i];
		}
	}

	delete[] errorMsgs;
}
