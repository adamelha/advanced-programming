#include "cmd_parser.h"
#include "file_parser.h"

#define EXIT_SUCCESS	0
#define EXIT_FAILURE	-1


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
		fileParser.printErrorMsg();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

