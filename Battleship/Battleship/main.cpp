#include "cmd_parser.h"
#include "file_parser.h"
#include "board.h"
#include "status.h"
#include "battle.h"
#include "macros.h"
#include "display.h"
using namespace std;
int main(int argc, char **argv) {
	status_t status;

	// Parse command line args
	CmdParser cmd = CmdParser(argc, argv);
	status = cmd.parse();
	if (status != STATUS_OK)
	{
		return EXIT_FAIL;
	}

	Display display;
	// Set Default text color
	//display.setTextColor(TEXT_COLOR_DEFAULT);
	//display.hideCursor();
	string filesPath = cmd.getFilesPath();

	// Parse files to strings
	FileParser fileParser = FileParser(filesPath, PARSE_TYPE_BOARD);
	status = fileParser.parse();
	if (status != STATUS_OK)
	{
		fileParser.printErrorMsg();
		return EXIT_FAIL;
	}

	//TODO: Right now if some file parser error occres we do not continue to check board validity. Need to?
	// Check board validity and parse
	try
	{
		Board board = Board(fileParser.getBoard());
	
		status = board.parse();
		if (status != STATUS_OK)
		{
			if (status == STATUS_INVALID_BOARD) board.printErrorMsg();
		
			return EXIT_FAIL;
		}


	Battle battle(display, cmd);
	battle.War(filesPath, board);


		return EXIT_SUCCESS;
	}
	catch (BoardBadDimensions& bd)
	{
		ERROR_PRINT("Wrong board dimensions.\n", EXIT_FAIL);
	}
}

