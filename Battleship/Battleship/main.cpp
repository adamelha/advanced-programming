#include "cmd_parser.h"
#include "file_parser.h"
#include "board.h"
#include "status.h"
#include "battle.h"

#define EXIT_SUCCESS	0
#define EXIT_FAIL		-1


using namespace std;
int main(int argc, char **argv) {
	status_t status;

	// Parse command line args
	CmdParser cmd = CmdParser(argc, argv);
	cmd.parse();
	string filesPath = cmd.getFilesPath();

	// Parse files to strings
	FileParser fileParser = FileParser(filesPath);
	status = fileParser.parse();
	if (status != STATUS_OK)
	{
		fileParser.printErrorMsg();
		return EXIT_FAIL;
	}

	//TODO: Right now if some file parser error occres we do not continue to check board validity. Need to?
	
	// Check board validity and parse
	Board board = Board(fileParser.getBoard());
	
	status = board.parse();
	if (status != STATUS_OK)
	{
		if (status == STATUS_INVALID_BOARD) board.printErrorMsg();
		
		return EXIT_FAIL;
	}
	Battle battle;
	battle.War(fileParser, board);


	return EXIT_SUCCESS;
}

