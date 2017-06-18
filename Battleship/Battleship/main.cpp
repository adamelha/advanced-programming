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
	
	//Board board;
	std::vector<Board> boards;
	//TODO: Right now if some file parser error occres we do not continue to check board validity. Need to?
	// Check board validity and parse
	while (status != STATUS_NO_MORE_BOARDS)
	{
		try
		{
			Board board = Board(fileParser.getBoard());
			status = board.parse();
			if (status != STATUS_OK)
			{
				if (status == STATUS_INVALID_BOARD) 
					board.printErrorMsg();
				throw BoardBadDimensionsOrHasMoreBoardsInFile();  // bad dimentions- serch next board
				
			}
			boards.push_back(board);
			throw BoardBadDimensionsOrHasMoreBoardsInFile();  //go to next board in file
		}
		catch (BoardBadDimensionsOrHasMoreBoardsInFile& bd)
		{
			DEBUG_PRINT("Bad Board.\n");
			
			// Try to parse next board file
			status = fileParser.parse();
			if (status != STATUS_OK)
			{
				fileParser.printErrorMsg();
			}
		}
	}

	try
	{
		ThreadManager threadManager(filesPath, boards, threadNumber);
		threadManager.run();
		 
	}
	catch (notEnoughDlls& e)
	{
		return EXIT_FAIL;
	}

	return EXIT_SUCCESS;
}

