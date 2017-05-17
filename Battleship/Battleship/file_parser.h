#pragma once

#include <string>
#include "error_class.h"
#include "status.h"
#include "macros.h"
#define BOARD_REGEX				"*.sboard"
#define ATTACK_REGEX			"*.attack"
#define DLL_REGEX				"*.dll"



#define WRONG_PATH_MSG			"Wrong path: " + this->filesPath + "\n"
#define WRONG_PATH_IDX			0

#define MISSING_BOARD_MSG		"Missing board file (" + this->boardFileName + ") looking in path: " + this->filesPath + "\n"
#define MISSING_BOARD_IDX		1

#define MISSING_ATTACK_MSG		"Missing attack file for player (*.attack) looking in path: " + this->filesPath + "\n"
#define MISSING_DLL_MSG		"Missing dll file for player (*.dll) looking in path: " + this->filesPath + "\n"

#define MISSING_ATTACK_IDX		2
#define MISSING_DLL_IDX		3


#define NUM_OF_FILE_PARSER_ERR_MSGS			4

#define MAX_MSG_SIZE			256

typedef enum parseType { PARSE_TYPE_PLAYER_A = PLAYER_A, PARSE_TYPE_PLAYER_B = PLAYER_B, PARSE_TYPE_BOARD} parseType_e;

using namespace std;


class FileParser : public ErrorClass {
public:
	status_t parse();
	string getBoard();
	string getAttack();
	string getAttackFileName() const;
	FileParser(string filesPath, parseType_e parseType) : ErrorClass(NUM_OF_FILE_PARSER_ERR_MSGS), filesPath(filesPath), parseType(parseType){}
	status_t parseAttack();
private:
	status_t parsePaths();
	status_t parseFiles();
	void pathToFileString(string path, string &fileStringOut);
	parseType_e parseType;
	const string filesPath;
	string boardFileName, attackFileName , dllFileName;
	string boardPath, attackPath, dllPath1 , dllPath2;

	// Content of the file as a c++ string
	string board, attack;
};
