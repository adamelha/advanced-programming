#pragma once

#include <string> // Actually not required here - but here due to students' request

/*Notice:

enum class AttackResult
{
	Miss,
	Hit,
	Sink
};

struct Coordinate
{
	int row, col, depth;
	Coordinate(int row_, int col_, int depth_) : row(row_), col(col_), depth(depth_) {}
};

/*This is a wrapper to the Board's Data
// IBattleshipGameAlgo for Ex2 - do not change this class - you should use it as is
class BoardData
{
public:
	virtual ~BoardData() = default;
	virtual char charAt(Coordinate c) const = 0; //returns only selected players' chars

	int rows()  const { return _rows; }
	int cols()  const { return _cols; }
	int depth() const { return _depth; }
protected:
	int _rows = 0; // make sure you set all protected members in the derived class.
	int _cols = 0; // make sure you set all protected members in the derived class.
	int _depth = 0; // make sure you set all protected members in the derived class.
};

class IBattleshipGameAlgo
{
public:
	virtual ~IBattleshipGameAlgo() = default;
	virtual void setPlayer(int player) = 0;				// called every time the player changes his order
	virtual void setBoard(const BoardData& board) = 0;	// called once at the beginning of each new game
	virtual Coordinate attack() = 0;					// ask player for his move
	virtual void notifyOnAttackResult(int player, Coordinate move, AttackResult result) = 0; // last move result
};

#ifdef ALGO_EXPORTS										// A flag defined in this project's Preprocessor's Definitions
#define ALGO_API extern "C" __declspec(dllexport)		// If we build - export
#else
#define ALGO_API extern "C" __declspec(dllimport)		// If someone else includes this .h - import
#endif

/* Please Notice:
ALGO_API IBattleshipGameAlgo* GetAlgorithm(); // This method must be implemented in each player(algorithm) .cpp file
