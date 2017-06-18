#include "cmd_parser.h"
#include "file_parser.h"
#include "board.h"
#include "status.h"
#include "battle.h"
#include "macros.h"
#include "thread_manager.h"

using namespace std;

int main(int argc, char **argv) {
	status_t status;
	int threadNumber;

	// Parse command line args
	CmdParser cmd = CmdParser(argc, argv);
	status = cmd.parse();
	
	if (status != STATUS_OK)
	{
		return EXIT_FAIL;
	}

	
	threadNumber = cmd.getThreadNumber();

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
	while (true)
	{
		try
		{
			Board board = Board(fileParser.getBoard());

			status = board.parse();
			if (status != STATUS_OK)
			{
				if (status == STATUS_INVALID_BOARD) board.printErrorMsg();

				throw BoardBadDimensions();
			}

			ThreadManager threadManager(filesPath, board, threadNumber);
			threadManager.run();
			
			while (true) {};
			return EXIT_SUCCESS;
		}
		catch (BoardBadDimensions& bd)
		{
			DEBUG_PRINT("Bad Board.\n");
			
			// Try to parse next board file
			status = fileParser.parse();
			if (status != STATUS_OK)
			{
				fileParser.printErrorMsg();
				return EXIT_FAIL;
			}
		}
		catch (notEnoughDlls& e) 
		{
			return EXIT_FAIL;
		}
	}
}

