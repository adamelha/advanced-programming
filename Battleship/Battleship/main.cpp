#include "cmd_parser.h"
#include "file_parser.h"
#include "board.h"
#define EXIT_SUCCESS	0
#define EXIT_FAIL	-1


using namespace std;
int main(int argc, char **argv) {

	// Parse command line args
	CmdParser cmd = CmdParser(argc, argv);
	cmd.parse();
	string filesPath = cmd.getFilesPath();

	// Parse files to strings
	FileParser fileParser = FileParser(filesPath);
	fileParser_t fileParserStatus = fileParser.parse();
	if (fileParserStatus != FILE_PARSER_OK)
	{
		//TODO: make printErrorMsg() in seperate class and have FileParser and Board inherit from it??
		fileParser.printErrorMsg();
		return EXIT_FAIL;
	}

	//TODO: Right now if some file parser error occres we do not continue to check board validity. Need to?
	
	// Check board validity and parse
	Board_t boardStatus;
	Board board = Board(fileParser.getBoard());
	
	boardStatus = board.parse();
	if (boardStatus != BOARD_OK)
	{
		if (boardStatus == BOARD_FILE_NOT_VALID) board.printErrorMsg();
		
		return EXIT_FAIL;
	}

	return EXIT_SUCCESS;
}

