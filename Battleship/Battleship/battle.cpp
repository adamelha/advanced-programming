#include "battle.h"
#include <stdlib.h>
#define EXIT_SUCCESS	0
#define EXIT_FAIL		-1
#define VALID_INT_TOKENS  -1 < stoi(tokens[0]) < 10 &&  -1 < stoi(tokens[1]) < 10
//#define IS_CHAR_BELONG_TO_A_BOARD     board[i][j] == 'B' || board[i][j] == 'P' || board[i][j] == 'M' || board[i][j]=='D'  
//#define IS_CHAR_BELONG_TO_B_BOARD     board[i][j] == 'b' || board[i][j] == 'p' || board[i][j] == 'm' || board[i][j]=='d'  

// this method from Tirgul 3 

int isBelongToBoard(char c ) 
{
	if (c == 'B' || c == 'P' || c == 'M' || c == 'D' )         //A Square
		return 1;
	else if (c == 'b' || c == 'p' || c == 'm' || c == 'm')     // B sqare
		return 2;
	else {
		return 0;
	}
}
static std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	std::stringstream strSt(s);
	std::string item;

	while (std::getline(strSt, item, delim)) //we could ommit the 'std::' notion from here.
	{
		elems.push_back(item); //push_back is a function of vector class, it inserted to the back of the vector.
	}

	return elems;
}


void Battle::setLineOrColumn(char board[][ARR_SIZE], int size , Point* p , bool isVertical, char symbol )
{
	if(isVertical)
	{
		for(int i =0 ; i < size ; i++)
			board[p->x][p->y + i ] = symbol;	
	}
	
	else
	{
		for (int i = 0; i < size; i++)
			board[p->x + i][p->y] = symbol;	
	}
}

void Battle::setAttacker(string attackStr, int whosTurn)
{
	vector<string> attacker = split(attackStr, '\n');
	if (whosTurn)		//B attack 
	{
		for(int i = 0; i < attacker.size(); i++)
		{
			
		}

	}
	else                //A attack
	{



	}
}

void Battle::processLine(const string & line, int whosTurn)
{
	vector<string> tokens = split(line, ',');
	if (tokens.size() != 2 || !VALID_INT_TOKENS)
	{
		return;
	}
	pair<int, int> aPair(stoi(tokens[0]), stoi(tokens[1]));
	if (whosTurn)								// B
		B_Atacker.push_back(aPair);
	else
		A_Atacker.push_back(aPair);
	
	
}

void Battle::loadFromAttackFile(const string & attackPath, int whosTurn)
{
	if(whosTurn)
		this->B_Atacker.clear();		//clears the vector
	else
		this->A_Atacker.clear();
	ifstream fin(attackPath);			//creating an ifstream object and opening file in path attackPath 
	string line;
	while (getline(fin, line))		   //getline returns null if we get to end of file.
	{
		this->processLine(line, whosTurn);
	}
}



void Battle::setFilePath(string path)
{
	this->filesPath = path;
}

void Battle::setWhosTurn(int turn)
{
	this->whosTurn = turn;
}

void Battle::setBoard(const char** board, int numRows, int numCols)
{	
	if (this->whosTurn)					//set Board to Player B
	{
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCols; j++)
			{
				if (isBelongToBoard(board[i][j]) == 2)        
				{
					this->B_Board[i][j] = board[i][j];
					numOfSquareB += 1;
				}
				else
					this->B_Board[i][j] = ' ';       // initialize board to 
			}
		}
	}
	
	else                                //set Board to Player A
	{
		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCols; j++)
			{
				if (isBelongToBoard(board[i][j]) == 1)
				{
					this->A_Board[i][j] = board[i][j];
					numOfSquareA += 1;
				}
				else
					this->A_Board[i][j] = ' ';
			}
		}
	}


/*	this->whosTurn;
	int status;
	FileParser fileParser = FileParser(filesPath);
	status = fileParser.parse();
	Board board2 = Board(fileParser.getBoard());
	status = board2.parse();
	board2.shipListA;
	Point* p;
	
	if (this->whosTurn)    // player B 
	{
		//this->attack = split(fileParser.getAttackA(), '\n');
		for (int i = 0; i < 5 ; i ++)
		{
			p = board2.shipListB[i]->pointList;
			p->y += 1;											// for checking verticallity
			this->numOfSquareB += board2.shipListB[i]->size;	

			if( board2.shipListB[i]->size > 1 && board2.getCharFromBoard(p) == board2.shipListB[i]->charSymbol )
				setLineOrColumn(this->B_Board, board2.shipListB[i]->size, board2.shipListB[i]->pointList , true , board2.shipListB[i]->charSymbol);
			else
				setLineOrColumn(this->B_Board, board2.shipListB[i]->size, board2.shipListB[i]->pointList, false, board2.shipListB[i]->charSymbol);
		}
	}

	else				  //player A
	{
		for (int i = 0; i < 5; i++)
		{
			p = board2.shipListA[i]->pointList;
			p->y += 1;										   // for checking verticallity
			this->numOfSquareA += board2.shipListA[i]->size;
			if (board2.shipListA[i]->size > 1 && board2.getCharFromBoard(p) == board2.shipListA[i]->charSymbol)
				setLineOrColumn(this->A_Board, board2.shipListA[i]->size, board2.shipListA[i]->pointList, true, board2.shipListA[i]->charSymbol);
			else
				setLineOrColumn(this->A_Board, board2.shipListA[i]->size, board2.shipListA[i]->pointList, false, board2.shipListA[i]->charSymbol);
		}
	}
	*/
}

std::pair<int, int> Battle::attack()
{
	return std::pair<int, int>();
}

void Battle::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
}

int Battle::War(FileParser fileParser, Board board)
{
	//initialize full const board for method setBoard( memory allocation included )
	char fullBoard[ARR_SIZE][ARR_SIZE];
	for (int i = 0 ; i < ARR_SIZE ; i++)
	{
		for (int j = 0; j < ARR_SIZE; j++)
			fullBoard[i][j] = board.getCharFromBoard(i, j);
	}
	const char ** constFullBoard = (const char**)calloc(ARR_SIZE, sizeof(char*));
	for (int i = 0; i < ARR_SIZE; i++)
		constFullBoard[i] = (char*)calloc(ARR_SIZE, sizeof(char));
	for (int i = 0 ; i < ARR_SIZE; i++)
		constFullBoard[i] = fullBoard[i];
	

	//set each player board
	setWhosTurn(0);						 //   set turn A
	setBoard(constFullBoard, 10, 10);
	setWhosTurn(1);						//    set turn B
	setBoard(constFullBoard, 10, 10);
	
	//set players attack vectors
	loadFromAttackFile(fileParser.getAttackAFileName(), 0);
	loadFromAttackFile(fileParser.getAttackBFileName(), 1);

	//war!!
	setWhosTurn(0);						 //   set turn A
	bool twoPlayersOutOfPlays = false , alreadyGotHit = false ,HitCorrectTarget = false;
	int indexA = 0, indexB = 0;
	int whoGotHit;
	 
	int total = this->B_Atacker.size() + this->A_Atacker.size();
	while ( this->numOfSquareA > 0 && this->numOfSquareB > 0  && !twoPlayersOutOfPlays)
	{
		whoGotHit = isBelongToBoard(fullBoard[this->B_Atacker[indexB].first][this->B_Atacker[indexB].second]);
		if (this->whosTurn)			//player B
		{
			if (whoGotHit == 0 )                // miss
			{
				HitCorrectTarget = false;
				//need to notify for next assignments
			}
			else if (whoGotHit == 1)		  // player B Hit player A
			{
				alreadyGotHit = (this->A_Board[this->B_Atacker[indexB].first][this->B_Atacker[indexB].second] == '\n');
				if (!alreadyGotHit)
				{
					this->numOfSquareA -= 1;
					twoPlayersOutOfPlays = (total < indexA + indexB);
					this->A_Board[this->B_Atacker[indexB].first][this->B_Atacker[indexB].second] = '\n';   //mark has hit
					HitCorrectTarget = true;
					
				}
			}
			else							//B Hit himself!!
			{
				alreadyGotHit = (this->B_Board[this->B_Atacker[indexB].first][this->B_Atacker[indexB].second] == '\n');
				if (!alreadyGotHit)
				{
					this->numOfSquareB -= 1;
					twoPlayersOutOfPlays = (total < indexA + indexB + 1);
					this->B_Board[this->B_Atacker[indexB].first][this->B_Atacker[indexB].second] = '\n';   //mark has hit
					HitCorrectTarget = false;

				}
			}

			indexB += 1;
			if (indexA < this->A_Atacker.size() && !HitCorrectTarget)
				setWhosTurn((this->whosTurn + 1) % 2) ;          //change turn to next player if possibale (to player A)
		}
		else                       //player A
		{
			whoGotHit = isBelongToBoard(fullBoard[this->A_Atacker[indexA].first][this->A_Atacker[indexA].second]);
			if (whoGotHit == 0 )                // miss
			{
				HitCorrectTarget = false;
				//need to notify for next assignments
			}
			else if (whoGotHit == 2)		  // player A Hit player B
			{
				alreadyGotHit = (this->B_Board[this->A_Atacker[indexB].first][this->A_Atacker[indexB].second] == '\n');
				if (!alreadyGotHit)
				{
					this->numOfSquareB -= 1;
					twoPlayersOutOfPlays = (total < indexA + indexB);
					this->B_Board[this->B_Atacker[indexB].first][this->B_Atacker[indexB].second] = '\n';   //mark has hit
					HitCorrectTarget = true;

				}
			}
			else               //A Hit himself!!
			{
				alreadyGotHit = (this->A_Board[this->A_Atacker[indexB].first][this->A_Atacker[indexB].second] == '\n');
				if (!alreadyGotHit)
				{
					this->numOfSquareA -= 1;
					twoPlayersOutOfPlays = (total < indexA + indexB + 1);
					this->A_Board[this->B_Atacker[indexB].first][this->B_Atacker[indexB].second] = '\n';   //mark has hit
					HitCorrectTarget = false;


				}
			}


			indexA += 1;
			if (indexB < this->B_Atacker.size() && !HitCorrectTarget)
				setWhosTurn((this->whosTurn + 1) % 2);          //change turn to next player if possibale and the player missed (to player B)
		}
	}
	
	
	//game is over, print winners if there are
	if (this->numOfSquareB == 0)
	{
		std::cout << "Player <A> won" << std::endl; 
	}
	else if(this->numOfSquareA == 0)
	{
		std::cout << "Player <A> won" << std::endl;
	}
	else
		std::cout << "nobody won" << std::endl;


	//free allocation
	for (int i  = 0; i < ARR_SIZE; i++)
	{
		free((void*)constFullBoard[i]);
	}

	free(constFullBoard);

	return 0;
}









