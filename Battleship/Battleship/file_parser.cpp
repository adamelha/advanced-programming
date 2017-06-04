#include "file_parser.h"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <Windows.h>
#include "macros.h"
#include "status.h"
#include <string>
#include <AtlBase.h>
#include <atlconv.h>


// Credit stackoverflow
static std::wstring stringToWstring(std::string str)
{
	wstring ws;
	CA2W ca2w(str.c_str());
	ws = ca2w;

	return ws;
}

static std::string wstringTostring(std::wstring wstr)
{
	CW2A cw2a(wstr.c_str());
	string s = cw2a;

	return s;
}



/***************Public Methods***************/

// Parse files
status_t FileParser::parse()
{
	status_t status;

	status = this->parsePaths();
	if (status != STATUS_OK) return status;

	status = this->parseFiles();
	return status;

}


status_t FileParser::parseAttack()
{
	status_t status;

	status = this->parsePaths();
	if (status != STATUS_OK) return status;

	status = this->parseFiles();
	return status;

}

// Getters of content of files
string FileParser::getBoard()
{
	return this->board;
}


string FileParser::getAttack()
{
	return this->attack;
}

string FileParser::getAttackFileName() const
{
	return this->attackPath;
}




/***************Private Methods***************/
status_t FileParser::parsePaths()
{
	WIN32_FIND_DATAA FindFileData, FindFileDataDll;
	HANDLE hFind;
	DWORD ftyp;
	string boardAbsPath, attackAbsPath, dllAbsPath, slash;
	status_t status = STATUS_OK;
	bool fileStatus;
	// If path is relative or ends with '\', no need to add '\\' 
	if (this->filesPath.length() == 0 || this->filesPath[this->filesPath.length() - 1] == '\\')
	{
		slash = "";
	}
	else
	{
		slash = "\\";
	}

	boardAbsPath = (this->filesPath) + slash + BOARD_REGEX;
	attackAbsPath = (this->filesPath) + slash + ATTACK_REGEX;
	dllAbsPath = (this->filesPath) + slash + DLL_REGEX;
	// If path is not working dir - check that it exists
	if (this->filesPath != "")
	{
		//check that path exists - if not, or is not directory, return without checking anything else
		ftyp = GetFileAttributesA(this->filesPath.c_str());
		if (ftyp == INVALID_FILE_ATTRIBUTES || !(ftyp & FILE_ATTRIBUTE_DIRECTORY))
		{
			addErrorMsg(WRONG_PATH_IDX, WRONG_PATH_MSG);
			return STATUS_ERROR;
		}
	}
	//TODO : must parse and check that path exists 

	// Parse board path
	if (parseType == PARSE_TYPE_BOARD)
	{
		hFind = FindFirstFileA((boardAbsPath).c_str(), &FindFileData);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			status = STATUS_ERROR;
			addErrorMsg(MISSING_BOARD_IDX, MISSING_BOARD_MSG);
		}
		else {
			//DEBUG_PRINT("The first board file found is %s\n", wstringTostring(FindFileData.cFileName).c_str());
			this->boardFileName = FindFileData.cFileName;
			this->boardPath = string(this->filesPath + slash + this->boardFileName);
		}

		FindClose(hFind);
		// Parse dll first  path
		hFind = FindFirstFileA((dllAbsPath).c_str(), &FindFileDataDll);

		string dll1Name, dll2Name;

		if (hFind == INVALID_HANDLE_VALUE)
		{
			status = STATUS_ERROR;
			addErrorMsg(MISSING_DLL_IDX, MISSING_DLL_MSG);
		}
		else 
		{
			dll1Name = string(FindFileDataDll.cFileName);
			DEBUG_PRINT("The first dll file found is %s\n", FindFileDataDll.cFileName);
			this->dllFileName = FindFileDataDll.cFileName;
			this->dllPath1 = string(this->filesPath + slash + this->dllFileName);
		}
		// Parse dll second 
		fileStatus = FindNextFileA(hFind, &FindFileDataDll);
		if (!fileStatus)
		{
			status = STATUS_ERROR;
			addErrorMsg(MISSING_DLL_IDX, MISSING_DLL_MSG);
		}
		else
		{
			DEBUG_PRINT("The second dll file found is %s\n", FindFileDataDll.cFileName);
			this->dllFileName = FindFileDataDll.cFileName;
			this->dllPath2 = string(this->filesPath + slash + this->dllFileName);
		}


			


		
		FindClose(hFind);
		return status;
	}

		// Parse attacker A path
		hFind = FindFirstFileA(attackAbsPath.c_str(), &FindFileData);

		if (hFind == INVALID_HANDLE_VALUE)
		{
			status = STATUS_ERROR;
			addErrorMsg(MISSING_ATTACK_IDX, MISSING_ATTACK_MSG);
		}
		else {
			DEBUG_PRINT("The first attacker file found is %s\n", FindFileData.cFileName);
			if (parseType == PARSE_TYPE_PLAYER_A) {
				this->attackFileName = FindFileData.cFileName;
				this->attackPath = string(this->filesPath + slash + this->attackFileName);
			}
		}


		// Parse attacker B path - second file with the same regex
		if (parseType == PARSE_TYPE_PLAYER_B)
		{
			//fileStatus = FindNextFileA(hFind, &FindFileData);
			if (hFind == INVALID_HANDLE_VALUE)
			{
				status = STATUS_ERROR;
				addErrorMsg(MISSING_ATTACK_IDX, MISSING_ATTACK_MSG);
			}
			else {
				DEBUG_PRINT("The second attacker file found is %s\n", FindFileData.cFileName);
				this->attackFileName = FindFileData.cFileName;
				this->attackPath = string(this->filesPath + slash + this->attackFileName);
			}
		}
		FindClose(hFind);
		return status;
}



//TODO: must return fail status in case of failure
status_t FileParser::parseFiles()
{
	if (parseType == PARSE_TYPE_BOARD) {
		pathToFileString(this->boardPath, this->board);
	}
	else
	{
		pathToFileString(this->attackPath, this->attack);
	}

	return STATUS_OK;
}

//TODO: must return fail status in case of failure
void FileParser::pathToFileString(string path, string &fileStringOut)
{
	std::ifstream t(path);
	std::string str;
	t.seekg(0, std::ios::end);
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	fileStringOut = string(str);
}
