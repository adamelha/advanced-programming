#include "error_class.h"
#include <iostream>
#include <fstream>

// Default does not print
ErrorClass::ErrorClass(size_t num_of_err_msgs) : ErrorClass(num_of_err_msgs, ErrorPrintType::NoPrint)
{
	
}

ErrorClass::ErrorClass(size_t num_of_err_msgs, ErrorPrintType type) : printType(type), num_of_err_msgs(num_of_err_msgs), logFilePath(DEFAULT_LOG_FILE)
{
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
			printSingleMsg(errorMsgs[i]);
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

void ErrorClass::printSingleMsg(string msg)
{
	std::ofstream outfile;

	switch (printType)
	{
	case ErrorPrintType::NoPrint:
		break;
	case ErrorPrintType::CoutPrint:
		cout << msg;
		break;
	case ErrorPrintType::LogfilePrint:
		outfile.open(logFilePath, std::ios_base::app);
		outfile << msg;
		break;
	default:
		break;
	}
}