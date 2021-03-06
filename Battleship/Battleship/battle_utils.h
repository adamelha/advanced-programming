#pragma once
#include <iostream>
#include <vector>
typedef enum player { player_A = 0, player_B = 1 } player_is;
typedef enum squareType { NO_SQUARE = 0, A_SQUARE = 1, B_SQUARE = 2 } squareType_e;
int isBelongToBoard(const char c);
template <class T>
T*** alloc3dArray(int rows, int cols, int depth);

template <class T>
void init3dArray(T ***arr, T value, int rows, int cols, int depth);

template <class T>
void copy3dArray(T ***dst, T ***src, int rows, int cols, int depth);

template <class T>
void alloc3dVector(std::vector < std::vector< std::vector<T> > > & arr, int rows, int cols, int depth);

template <class T>
void init3dVector(std::vector < std::vector< std::vector<T> > > &arr, T value, int rows, int cols, int depth);

template <class T>
void copy3dVector(std::vector < std::vector< std::vector<T> > > &dst, std::vector < std::vector< std::vector<T> > >src, int rows, int cols, int depth);


template <class T>
T*** alloc3dArray(int rows, int cols, int depth)
{
	// Init board - a dynamic 3 dimensional array (credit Stackoverflow)
	T ***arr = new T**[rows];
	for (int i = 0; i < rows; ++i)
	{
		arr[i] = new T*[cols];

		for (int k = 0; k < cols; ++k)
			arr[i][k] = new T[depth];
	}

	return arr;
}

// Set all members of 3d arr to value
template <class T>
void init3dArray(T ***arr, T value, int rows, int cols, int depth)
{
	for (size_t x = 0; x < rows; x++)
	{
		for (size_t y = 0; y < cols; y++)
		{
			for (size_t z = 0; z < depth; z++)
			{
				arr[x][y][z] = value;
			}
		}
	}
}

// Copy from on board to another. Dimensions must match
template <class T>
void copy3dArray(T ***dst, T ***src, int rows, int cols, int depth)
{
	for (size_t x = 0; x < rows; x++)
	{
		for (size_t y = 0; y < cols; y++)
		{
			for (size_t z = 0; z < depth; z++)
			{
				dst[x][y][z] = src[x][y][z];
			}
		}
	}
}



template <class T>
void alloc3dVector(std::vector < std::vector< std::vector<T> > > & arr, int rows, int cols, int depth)
{
	// Init board - a dynamic 3 dimensional array (credit Stackoverflow)
	//std::vector < std::vector < std::vector<T> arr; 
	arr.resize(rows);
	for (int i = 0; i < rows; ++i)
	{
		arr[i].resize(cols);

		for (int k = 0; k < cols; ++k)
			arr[i][k].resize(depth);
	}

	//return arr;
}

template <class T>
void init3dVector(std::vector < std::vector< std::vector<T> > > &arr, T value, int rows, int cols, int depth)
{
	for (size_t x = 0; x < rows; x++)
	{
		for (size_t y = 0; y < cols; y++)
		{
			for (size_t z = 0; z < depth; z++)
			{
				arr[x][y][z] = value;
			}
		}
	}
}


// Copy from on board to another. Dimensions must match
template <class T>
void copy3dVector(std::vector < std::vector< std::vector<T> > > &dst, std::vector < std::vector< std::vector<T> > >src, int rows, int cols, int depth)
{
	for (size_t x = 0; x < rows; x++)
	{
		for (size_t y = 0; y < cols; y++)
		{
			for (size_t z = 0; z < depth; z++)
			{
				dst[x][y][z] = src[x][y][z];
			}
		}
	}
}
