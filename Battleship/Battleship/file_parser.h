#pragma once

#include <string>
#include "error_class.h"
#include "status.h"
#define BOARD_REGEX				"*.sboard"
#define ATTACK_A_REGEX			"*.attack-a"
#define ATTACK_B_REGEX			"*.attack-b"



#define WRONG_PATH_MSG			"Wrong path: " + this->filesPath + "\n"
#define WRONG_PATH_IDX			0

#define MISSING_BOARD_MSG		"Missing board file (" + this->boardFileName + ") looking in path: " + this->filesPath + "\n"
#define MISSING_BOARD_IDX		1

#define MISSING_ATTACK_A_MSG	"Missing attack file for player A (" + this->attackAFileName + ") looking in path: " + this->filesPath + "\n"
#define MISSING_ATTACK_A_IDX	2

#define MISSING_ATTACK_B_MSG	"Missing attack file for player B (" + this->attackAFileName + ") looking in path: " + this->filesPath + "\n"
#define MISSING_ATTACK_B_IDX	3

#define NUM_OF_FILE_PARSER_ERR_MSGS			4

#define MAX_MSG_SIZE			256
using namespace std;


class FileParser : public ErrorClass {
public:
	status_t parse();
	string getBoard();
	string getAttackA();
	string getAttackB();
	string getAttackAFileName() const;
	string getAttackBFileName() const;
	FileParser(string filesPath) : ErrorClass(NUM_OF_FILE_PARSER_ERR_MSGS), filesPath(filesPath) {}

private:
	status_t parsePaths();
	status_t parseFiles();
	void pathToFileString(string path, string &fileStringOut);

	string filesPath, boardFileName, attackAFileName, attackBFileName;
	string boardPath, attackAPath, attackBPath;

	// Content of the file as a c++ string
	string board, attackA, attackB;
};