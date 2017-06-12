#include "gameManager.h"

gameManager::gameManager() {

	for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {

		numberOfShipsLeft[i] = 5;
		scores[i] = 0;
		playerHasMoves[i] = true;

		for (int j = 0; j < INITIAL_SHIPS_NUMBER; j++)
			fleet[i][j] = list<pair<int, int>>();
	}

	nextMovePlayerIndex = FIRST_PLAYER_INDEX;
}

gameManager::~gameManager() {

	for (auto it = players.begin(); it != players.end(); ++it)
		delete (*it);

}

bool gameManager::loadBoard(char** board, const string& path) {

	char boardFile[BUFFER_LENGTH];
	ifstream fs;
	string line;
	string representiveCharacters = "bBpPmMdD";
	char character;
	FILE* fp;

	//create a list of all *.sboard files in the directory
	fp = _popen("dir /b /a-d *.sboard 2> nul", "r");

	//get the first file name on the list of *.sboard files
	if (fgets(boardFile, BUFFER_LENGTH - 1, fp) == NULL) {
		cout << "Missing board file (*.sboard) looking in path: " << path << endl;
		_pclose(fp);
		return false;
	}

	_pclose(fp);
	if (boardFile[strlen(boardFile) - 1] == '\n')
		boardFile[strlen(boardFile) - 1] = '\0';

	//open board file in reading mode
	fs.open(boardFile, fstream::in);

	for (int i = 0; i < BOARD_EDGE_LENGTH; i++) {

		//get the i'th row considering either /n or /r/n as a new line delimiter
		getline(fs, line);

		for (int j = 0; j < min((int)line.length(), BOARD_EDGE_LENGTH); j++) {

			character = line.c_str()[j];

			//if its a valid ship part write it to the board
			if (representiveCharacters.find(character) != string::npos)
				board[i][j] = character;
		}
	}

	fs.close();
	return true;
}

bool gameManager::isBoardValid(char** board) {

	int shipLength;
	bool result = true;
	char playerChar;
	char shipeTypes[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_TYPES] = {
		{ 'B', 'P', 'M' ,'D' },
		{ 'b', 'p', 'm', 'd' }
	};

	//0 indicates player A and 1 indicates player B
	int player;

	//counters of the battleships for each player
	int playersShipsNumber[NUMBER_OF_PLAYERS] = { 0, 0 };

	//indicate if a wrong size or shape ship was found for a specific player and ship type
	bool wrongSizeValidityArray[NUMBER_OF_PLAYERS][NUMBER_OF_SHIP_TYPES] = { { false } };

	//indicate if there are adjacent ships on board
	bool adjacentShipsOnBoard = false;

	//remember valid ship parts that has already been tested to avoid replication in counts
	bool boolBoard[BOARD_EDGE_LENGTH][BOARD_EDGE_LENGTH] = { { false } };

	/*first check if there are Adjacent Ships on Board*/
	if (isThereAdjacentShipsOnBoard(board))
		adjacentShipsOnBoard = true;

	//for each square in the board
	for (int i = 0; i < BOARD_EDGE_LENGTH; i++) {
		for (int j = 0; j < BOARD_EDGE_LENGTH; j++) {

			//if the square contain a part of an unknown ship
			if ((board[i][j] != ' ') && (boolBoard[i][j] == false)) {

				//get the corresponding player and expected ship length
				getPlayerAndExpectedLength(board[i][j], &player, &shipLength);

				//if the ship is valid update the corresponding counter
				if (validateSizeAndShapeOfAShip(board, boolBoard, i, j, shipLength))
					playersShipsNumber[player] += 1;

				//if the ship is not valid mark its type
				else
					wrongSizeValidityArray[player][shipLength - 1] = true;
			}
		}
	}

	for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {

		//determine the player character
		if (i == 0)
			playerChar = 'A';
		else
			playerChar = 'B';

		//print an error if there are invalid ships and update result
		for (int j = 0; j < NUMBER_OF_SHIP_TYPES; j++) {
			if (wrongSizeValidityArray[i][j]) {
				cout << "Wrong size or shape for ship " << shipeTypes[i][j] << " for player " << playerChar << endl;
				result = false;
			}
		}
	}

	for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {

		//determine the player character
		if (i == 0)
			playerChar = 'A';
		else
			playerChar = 'B';

		//print an error if the player has to many/few ships and update result
		if (playersShipsNumber[i] > VALID_NUMBER_OF_SHIPS) {
			cout << "Too many ships for player " << playerChar << endl;
			result = false;
		}
		else if (playersShipsNumber[i] < VALID_NUMBER_OF_SHIPS) {
			cout << "Too few ships for player " << playerChar << endl;
			result = false;
		}
	}

	//print an error if there are adjacent ships on board and update result
	if (adjacentShipsOnBoard) {
		cout << "Adjacent Ships on Board" << endl;
		result = false;
	}

	return result;
}

bool gameManager::dllsExist(const string& path) {

	char dllFile[BUFFER_LENGTH];
	FILE* fp;

	//create a list of all *.sboard files in the directory
	fp = _popen("dir /b /a-d *.dll 2> nul", "r");

	for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
		//get the i'th file name on the list of *.sboard files
		if (fgets(dllFile, BUFFER_LENGTH - 1, fp) == NULL) {
			cout << "Missing an algorithm (dll) file looking in path: " << path << endl;
			_pclose(fp);
			return false;
		}
	}

	return true;
}

bool gameManager::executeGame(const char** board, const char** playerABoard, const char** playerBBoard, const string& path, int delay, bool quiet) {

	std::pair<int, int> square;
	AttackResult attackResult;
	int lastMovePlayerIndex = nextMovePlayerIndex;
	int currentPlayer;

	//load the players algorithms
	if (!loadAndInitDllFilesAndSetBoards(path, playerABoard, playerBBoard))
		return false;

	//save battleships info in game manager internal data structure
	setFleets(board);

	if (!quiet)
		displayGame::displayBoard(board);

	while (playerHasMoves[FIRST_PLAYER_INDEX] || playerHasMoves[SECOND_PLAYER_INDEX]) {

		//ask player for his move
		square = (players[nextMovePlayerIndex])->attack();
		if (square == pair<int, int>(-1, -1)) {
			playerHasMoves[nextMovePlayerIndex] = false;
			nextMovePlayerIndex = 1 - nextMovePlayerIndex;
			continue;
		}
		else if (square.first < 1 || square.first > BOARD_EDGE_LENGTH || square.second < 1 || square.second > BOARD_EDGE_LENGTH) {
			nextMovePlayerIndex = 1 - nextMovePlayerIndex;
			continue;
		}

		currentPlayer = nextMovePlayerIndex;

		//actual attack execution by the game manager
		attackResult = executeAttack(square);

		if (!quiet)
			displayGame::displayAttack(attackResult, square, board, currentPlayer, delay);


		//notify on last move result
		(players[FIRST_PLAYER_INDEX])->notifyOnAttackResult(lastMovePlayerIndex, square.first, square.second, attackResult);
		(players[SECOND_PLAYER_INDEX])->notifyOnAttackResult(lastMovePlayerIndex, square.first, square.second, attackResult);

		//if a ship was sinked
		if (attackResult == AttackResult::Sink) {

			if (numberOfShipsLeft[FIRST_PLAYER_INDEX] == 0) {
				cout << "Player B won" << endl;
				break;
			}
			else if (numberOfShipsLeft[SECOND_PLAYER_INDEX] == 0) {
				cout << "Player A won" << endl;
				break;
			}
		}
	}

	cout << "Points:" << endl;
	cout << "Player A :" << scores[FIRST_PLAYER_INDEX] << endl;
	cout << "Player B :" << scores[SECOND_PLAYER_INDEX] << endl;

	return true;
}

void gameManager::setPlayersBoards(const char** board, char** playerABoard, char** playerBBoard) {

	char boardPart;

	for (int i = 0; i < BOARD_EDGE_LENGTH; i++) {
		for (int j = 0; j < BOARD_EDGE_LENGTH; j++) {
			boardPart = board[i][j];
			if (boardPart == 'B' || boardPart == 'P' || boardPart == 'M' || boardPart == 'D')
				playerABoard[i][j] = boardPart;
			else if (boardPart == 'b' || boardPart == 'p' || boardPart == 'm' || boardPart == 'd')
				playerBBoard[i][j] = boardPart;
		}
	}
}

bool gameManager::loadAndInitDllFilesAndSetBoards(const string& path, const char** playerABoard, const char** playerBBoard) {

	HANDLE dir;
	WIN32_FIND_DATAA fileData; //data struct for file
	string fileName, fullFileName;
	int playerNumber = 0;

	// define function of the type we expect
	typedef IBattleshipGameAlgo *(*GetAlgoFuncType)();
	GetAlgoFuncType getAlgoFunc;

	// iterate over *.dll files in path
	string s = "\\*.dll"; // only .dll endings
	dir = FindFirstFileA((path + s).c_str(), &fileData); // Notice: Unicode compatible version of FindFirstFile
	if (dir == INVALID_HANDLE_VALUE) //check if the dir opened successfully
		return false;

	do {

		fileName = fileData.cFileName;
		fullFileName = path + "\\" + fileName;

		// Load dynamic library
		HINSTANCE hDll = LoadLibraryA(fullFileName.c_str()); // Notice: Unicode compatible version of LoadLibrary
		if (!hDll) {
			cout << "Cannot load dll: " << fullFileName << endl;
			return false;
		}

		// Get function pointer
		getAlgoFunc = (GetAlgoFuncType)GetProcAddress(hDll, "GetAlgorithm");
		if (!getAlgoFunc) {
			cout << "Cannot load dll: " << fullFileName << endl;
			return false;
		}

		dllLibs.push_back(hDll);
		players.push_back(getAlgoFunc());

		//called once to notify player on his board
		if (playerNumber == FIRST_PLAYER_INDEX)
			(players[FIRST_PLAYER_INDEX])->setBoard(FIRST_PLAYER_INDEX, playerABoard, BOARD_EDGE_LENGTH, BOARD_EDGE_LENGTH);
		else
			(players[SECOND_PLAYER_INDEX])->setBoard(SECOND_PLAYER_INDEX, playerBBoard, BOARD_EDGE_LENGTH, BOARD_EDGE_LENGTH);

		//call the players init function
		if (!(players[playerNumber])->init(path)) {
			cout << "Algorithm initialization failed for dll: " << fullFileName << endl;
			return false;
		}

		playerNumber++;

	} while (FindNextFileA(dir, &fileData)); // Notice: Unicode compatible version of FindNextFile

	return true;
}

void gameManager::setFleets(const char** board) {

	string shipTypes;
	char character;

	for (int playerIndex = 0; playerIndex < NUMBER_OF_PLAYERS; playerIndex++) {

		//detemine the ship types of this player
		if (playerIndex == FIRST_PLAYER_INDEX)
			shipTypes = "BPMD";
		else
			shipTypes = "bpmd";

		//for each square on the board
		for (int i = 0; i < BOARD_EDGE_LENGTH; i++) {
			for (int j = 0; j < BOARD_EDGE_LENGTH; j++) {
				character = board[i][j];

				//if this square contain a part of a ship that belongs to this player add it to the player records
				if (shipTypes.find(character) != string::npos)
					addShipToFleet(i, j, character, playerIndex);
			}
		}
	}
}

void gameManager::addShipToFleet(int i, int j, char shipType, int playerIndex) {

	pair<int, int> square = make_pair(i + 1, j + 1);

	for (int k = 0; k < INITIAL_SHIPS_NUMBER; k++) {

		list<pair<int, int>> &list = fleet[playerIndex][k];

		//if the sqaure beside (to the left) this square contain a part of this ship add it to the player records
		if (find(list.begin(), list.end(), make_pair(i + 1, j)) != list.end()) {
			list.push_back(square);
			return;
		}
		//if one sqaure above this square contain a part of this ship add it to the player records
		else if (find(list.begin(), list.end(), make_pair(i, j + 1)) != list.end()) {
			list.push_back(square);
			return;
		}
	}

	//if its not part of a ship that was already found add it to the player records as a new ship and determine its value
	for (int k = 0; k < INITIAL_SHIPS_NUMBER; k++) {

		list<pair<int, int>> &list = fleet[playerIndex][k];
		if (list.size() == 0) {
			list.push_back(square);
			shipsValues[playerIndex][k] = getShipValue(shipType);
			return;
		}
	}
}

int gameManager::getShipValue(char character) {

	switch (character) {
	case 'b':
	case 'B':
		return 2;
	case 'p':
	case 'P':
		return 3;
	case 'm':
	case 'M':
		return 7;
	case 'd':
	case 'D':
		return 8;
	default:
		return 0;
	}
}

AttackResult gameManager::executeAttack(pair<int, int> square) {

	//for each ship in the players fleets
	for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
		for (int j = 0; j < INITIAL_SHIPS_NUMBER; j++) {

			//search for that square in the ship parts list
			list<pair<int, int>> &list = fleet[i][j];
			auto attackedShipPart = find(list.begin(), list.end(), square);

			//if the sqaure is a part of this ship
			if (attackedShipPart != list.end()) {

				//delete it from the ship parts list
				list.erase(attackedShipPart);

				//change the next move player if necessary
				if (playerHasMoves[1 - i])
					nextMovePlayerIndex = 1 - i;
				else
					nextMovePlayerIndex = i;

				//if it was the last part of the ship return Sink
				if (list.size() == 0) {
					numberOfShipsLeft[i] -= 1;
					scores[1 - i] += shipsValues[i][j];
					return AttackResult::Sink;
				}
				//if it wasn't the last part of the ship return Hit
				else
					return AttackResult::Hit;
			}
		}
	}

	//if the square doesn't contain a ship part change the next move player if necessary and return Miss
	if (playerHasMoves[1 - nextMovePlayerIndex])
		nextMovePlayerIndex = 1 - nextMovePlayerIndex;
	return AttackResult::Miss;
}

bool gameManager::isThereAdjacentShipsOnBoard(char** board) {

	//go through the rows of the board and check if there are two adjucent ship parts of different types
	for (int i = 0; i < BOARD_EDGE_LENGTH; i++) {
		for (int j = 0; j < BOARD_EDGE_LENGTH - 1; j++) {

			if ((board[i][j] != ' ') && (board[i][j + 1] != ' ') && (board[i][j] != board[i][j + 1]))
				return true;
		}
	}

	//go through the colomns of the board and check if there are two adjucent ship parts of different types
	for (int i = 0; i < BOARD_EDGE_LENGTH - 1; i++) {
		for (int j = 0; j < BOARD_EDGE_LENGTH; j++) {

			if ((board[i][j] != ' ') && (board[i + 1][j] != ' ') && (board[i][j] != board[i + 1][j]))
				return true;
		}
	}

	return false;
}

void gameManager::getPlayerAndExpectedLength(char shipType, int *player, int *expectedLength) {

	switch (shipType) {
	case 'B':
		*player = 0;
		*expectedLength = 1;
		break;
	case 'b':
		*player = 1;
		*expectedLength = 1;
		break;
	case 'P':
		*player = 0;
		*expectedLength = 2;
		break;
	case 'p':
		*player = 1;
		*expectedLength = 2;
		break;
	case 'M':
		*player = 0;
		*expectedLength = 3;
		break;
	case 'm':
		*player = 1;
		*expectedLength = 3;
		break;
	case 'D':
		*player = 0;
		*expectedLength = 4;
		break;
	case 'd':
		*player = 1;
		*expectedLength = 4;
		break;
	}
}

bool gameManager::validateSizeAndShapeOfAShip(char** board, bool boolBoard[BOARD_EDGE_LENGTH][BOARD_EDGE_LENGTH], int i, int j, int expectedLength) {

	if (shipSetHorizontally(board, boolBoard, i, j, expectedLength) || shipSetVertically(board, boolBoard, i, j, expectedLength))
		return true;
	else
		return false;
}

bool gameManager::shipSetHorizontally(char** board, bool boolBoard[BOARD_EDGE_LENGTH][BOARD_EDGE_LENGTH], int i, int j, int expectedLength) {

	char shipType = board[i][j];
	bool result = true;
	int k;

	//chack if there is enough space in board for horizontal setting of the ship starting from (i,j)
	if (BOARD_EDGE_LENGTH - expectedLength < j) {
		return false;
	}

	//check if the legth of the ship is at least expectedLength and update the boolBoard
	for (k = 0; k < expectedLength; k++) {

		if (board[i][j + k] == shipType)
			boolBoard[i][j + k] = true;
		else {
			result = false;
			break;
		}
	}

	//check if the legth of the ship is not longer then expectedLength
	//in case a problam was already detected nothing will be done
	if ((j + expectedLength < BOARD_EDGE_LENGTH) && (board[i][j + expectedLength] == shipType))
		result = false;
	if ((j > 0) && (board[i][j - 1] == shipType))
		result = false;

	//check if it's surounding contain a part from the same ship type
	//in case a problam was already detected nothing will be done
	for (int s = 0; s < expectedLength; s++) {

		if ((i + 1 < BOARD_EDGE_LENGTH) && (board[i + 1][j + s] == shipType))
			result = false;

		if ((i > 0) && (board[i - 1][j + s] == shipType))
			result = false;
	}

	//if the ship is invalid repair the boolBoard
	if (result == false) {
		for (int s = 0; s < k; s++)
			boolBoard[i][j + s] = false;
	}

	return result;
}

bool gameManager::shipSetVertically(char** board, bool boolBoard[BOARD_EDGE_LENGTH][BOARD_EDGE_LENGTH], int i, int j, int expectedLength) {

	char shipType = board[i][j];
	bool result = true;
	int k;

	//chack if there is enough space in board for vertical setting of the ship starting from (i,j)
	if (BOARD_EDGE_LENGTH - expectedLength < i) {
		return false;
	}

	for (k = 0; k < expectedLength; k++) {

		//check if the ship vertical length is valid and update the boolBoard
		if (board[i + k][j] == shipType)
			boolBoard[i + k][j] = true;
		else {
			result = false;
			break;
		}
	}

	//check if the legth of the ship is not longer then expectedLength
	//in case a problam was already detected nothing will be done
	if ((i + expectedLength < BOARD_EDGE_LENGTH) && (board[i + expectedLength][j] == shipType))
		result = false;
	if ((i > 0) && (board[i - 1][j] == shipType))
		result = false;

	//check if it's surounding contain a part from the same ship type
	//in case a problam was already detected nothing will be done
	for (int s = 0; s < expectedLength; s++) {

		if ((j + 1 < BOARD_EDGE_LENGTH) && (board[i + s][j + 1] == shipType))
			result = false;

		if ((j > 0) && (board[i + s][j - 1] == shipType))
			result = false;
	}

	//if the ship is invalid repair the boolBoard
	if (result == false) {
		for (int s = 0; s < k; s++)
			boolBoard[i + s][j] = false;
	}

	return result;
}

void gameManager::generatePath(int argc, char *argv[], string& path, int* delay, bool* quiet) {

	char* tmpPath = nullptr;
	char** ptr = argv;

	*delay = 100;
	*quiet = false;

	ptr++;
	if (argc > 1) {
		while (*ptr != NULL) {
			if (!strcmp(*ptr, "-quiet")) {
				*quiet = true;
				ptr++;
			}
			else if (!strcmp(*ptr, "-delay")) {
				*delay = atoi(ptr[1]);
				ptr += 2;
			}
			else {
				tmpPath = _fullpath(nullptr, ptr[0], ZERO_LENGTH);
				ptr++;
			}
		}
	}

	if (tmpPath == nullptr)
		tmpPath = _fullpath(nullptr, ".\\", ZERO_LENGTH);

	path = string(tmpPath);
	delete[] tmpPath;
}

char** gameManager::allocateBoard() {

	char ** board;

	board = new char*[BOARD_EDGE_LENGTH];
	for (int i = 0; i < BOARD_EDGE_LENGTH; i++) {
		board[i] = new char[BOARD_EDGE_LENGTH];
		for (int j = 0; j < BOARD_EDGE_LENGTH; j++) {
			board[i][j] = ' ';
		}
	}

	return board;
}

void gameManager::destroyBoards(char** board, char** playerABoard, char** playerBBoard) {

	for (int i = 0; i < BOARD_EDGE_LENGTH; i++) {
		delete[] board[i];
		delete[] playerABoard[i];
		delete[] playerBBoard[i];
	}
	delete[] board;
	delete[] playerABoard;
	delete[] playerBBoard;
}

int main(int argc, char *argv[]) {

	string path;
	gameManager Game;
	char** board = nullptr;
	char** playerABoard = nullptr;
	char** playerBBoard = nullptr;
	bool isBoardExist, isBoardValid, DllsExist;

	int delay;
	bool quiet;

	//calculate the path to the input files
	Game.generatePath(argc, argv, path, &delay, &quiet);

	//validate the directory existence and change dir to it
	if (_chdir(path.c_str())) {
		cout << "Wrong path: " << path << endl;
		return -1;
	}

	//initialize boards to spaces
	board = Game.allocateBoard();
	playerABoard = Game.allocateBoard();
	playerBBoard = Game.allocateBoard();

	//load the board return false iff board file doesn't exist
	isBoardExist = Game.loadBoard(board, path);

	//if the board exist check its validity
	if (isBoardExist)
		isBoardValid = Game.isBoardValid(board);

	//check dlls existence
	DllsExist = Game.dllsExist(path);

	if (!isBoardExist || !isBoardValid || !DllsExist) {
		Game.destroyBoards(board, playerABoard, playerBBoard);
		return -1;
	}

	//prepare the players boards in order to send them to setBoard
	Game.setPlayersBoards((const char**)board, playerABoard, playerBBoard);

	if (!Game.executeGame((const char**)board, (const char**)playerABoard, (const char**)playerBBoard, path, delay, quiet)) {
		Game.destroyBoards(board, playerABoard, playerBBoard);
		return -1;
	}

	Game.destroyBoards(board, playerABoard, playerBBoard);
	return 0;
}