#pragma once


//=======================================================

// Multi Dimensional matrix<T, dimensions>

// @author Amir Kirsh 

//=======================================================


#include <iostream>

#include <memory>

#include "point.h"

using std::cout;

using std::endl;


template<class T, size_t DIMENSIONS>

class Matrix;


template<class T, size_t DIMENSIONS>

struct MatrixCopier {

	static void copy(T* dest, size_t dest_size, const size_t* dest_dimensions, const T* source, size_t source_size, const size_t* source_dimensions) {

		size_t dest_size0 = dest_dimensions[0] ? dest_size / dest_dimensions[0] : 0;

		size_t source_size0 = source_dimensions[0] ? source_size / source_dimensions[0] : 0;

		for (size_t i = 0; i < source_dimensions[0]; ++i) {

			MatrixCopier<T, DIMENSIONS - 1>::copy(dest + (i * dest_size0), dest_size0, dest_dimensions + 1, source + (i * source_size0), source_size0, source_dimensions + 1);

		}

	}

};


template<class T>

struct MatrixCopier<T, 1> {

	static void copy(T* dest, size_t dest_size, const size_t* dest_dimensions, const T* source, size_t source_size, const size_t* source_dimensions) {

		for (size_t i = 0; i < source_size; ++i) {

			dest[i] = source[i];

		}

	}

};


template<class T, size_t DIMENSIONS>

struct MatrixPrinter {

	static void print(const T* values, size_t size, const size_t* dimensions, std::ostream& out = cout) {

		out << '{';

		size_t size0 = size / dimensions[0];

		MatrixPrinter<T, DIMENSIONS - 1>::print(values, size0, dimensions + 1, out);

		for (size_t i = 1; i < dimensions[0]; ++i) {

			out << ',';

			MatrixPrinter<T, DIMENSIONS - 1>::print(values + (i*size0), size0, dimensions + 1, out);

		}

		out << '}';

	}

};


template<class T>

struct MatrixPrinter<T, 1> {

	static void print(const T* values, size_t size, const size_t* dimensions, std::ostream& out = cout) {

		out << '{';

		out << values[0];

		for (size_t i = 1; i < size; ++i) {

			out << ',' << values[i];

		}

		out << '}';

	}

};


template<class T, size_t DIMENSIONS>

class Matrix {

	constexpr static size_t NUM_DIMENSIONS = DIMENSIONS;

	

	


	friend class Matrix<T, DIMENSIONS + 1>;

public:
	const size_t _size = 0;

	size_t _dimensions[DIMENSIONS] = {};
	std::unique_ptr<T[]> _array = nullptr;
	size_t size() const { return _size; }

	
	Matrix() {}

	T operator[] (Point<DIMENSIONS> p)
	{
		
		int idx = 0;
		int sizeOfSmallerList;
		for (size_t i = 0; i < DIMENSIONS ; i++)
		{
			sizeOfSmallerList = 1;
			for (size_t j = DIMENSIONS - 1; j > i; j--)
			{
				sizeOfSmallerList *= _dimensions[j];
			}
			idx += (sizeOfSmallerList * p.coordinates[i]);
		}

		return _array[idx];

	}
	

	// DIMENSIONS == 1

	// We want here a ctor with this signature:

	//    Matrix(const std::initializer_list<T>& values) {

	// but SFINAE is needed to block this ctor from being used by Matrix with DIMENSIONS > 1

	// The SFINAE results with the exact signature we want, but only for cases DIMENSIONS == 1

	template<typename G = T>

	Matrix(const std::initializer_list<typename std::enable_if_t<DIMENSIONS == 1, G>>& values) {

		const_cast<size_t&>(_size) = values.size();

		_dimensions[0] = _size;

		_array = std::make_unique<T[]>(_size);

		size_t i = 0;

		for (auto& val : values) {

			_array[i++] = val;

		}

	}

	// DIMENSIONS > 1

	// We want here a ctor with this signature:

	//    Matrix(const std::initializer_list<Matrix<T, DIMENSIONS-1>>& values) {

	// although this ctor is not useful and won't be used by Matrix<T, 1> it will still be in class

	// and thus would compile as part of Matrix<T, 1> resulting with a parameter of type:

	//          const std::initializer_list< Matrix<T, 0> >& values

	// having Matrix<T, 0> as a parameter - even for a function that is not in use, inside a class that is used

	// would require the compiler to instantiate Matrix<T, 0> class which results with a warning

	// the SFINAE below solves this warning.

	// The SFINAE results with the exact signature we want, but only for cases DIMENSIONS > 1

	template<typename G = T>

	Matrix(const std::initializer_list<Matrix<typename std::enable_if_t<(DIMENSIONS > 1), G>, DIMENSIONS - 1>>& values) {

		_dimensions[0] = values.size();

		for (auto& m : values) {

			for (size_t dim = 0; dim < DIMENSIONS - 1; ++dim) {

				if (m._dimensions[dim] > _dimensions[dim + 1]) {

					_dimensions[dim + 1] = m._dimensions[dim];

				}

			}

		}

		size_t size = 1;

		for (size_t dim = 0; dim < DIMENSIONS; ++dim) {

			size *= _dimensions[dim];

		}



		const_cast<size_t&>(_size) = size;

		_array = std::make_unique<T[]>(_size); // "zero initialized" - T()

		size_t i = 0;

		size_t dest_size = _size / _dimensions[0];

		for (auto& m : values) {

			MatrixCopier<T, DIMENSIONS - 1>::copy(&(_array[i * dest_size]), dest_size, _dimensions + 1, m._array.get(), m._size, m._dimensions);

			++i;

		}

	}


	Matrix(Matrix&& m) {

		*this = std::move(m);

	}

	// Copy 

	void copyFromLargerMatrix(const Matrix<T, DIMENSIONS + 1> &m, int idx) {
		size_t dest_size = _size / _dimensions[0];

		MatrixCopier<T, DIMENSIONS>::copy(&(_array[idx * dest_size]), dest_size, _dimensions + 1, m._array.get(), m._size, m._dimensions);
		
	}
	auto& operator=(Matrix&& m) {

		std::swap(_array, m._array);

		std::swap(const_cast<size_t&>(_size), const_cast<size_t&>(m._size));

		std::swap(_dimensions, m._dimensions);

		return *this;

	}


	size_t getDimension(size_t i) const {

		return _dimensions[i];

	}

	void iterate(int d, int n, size_t size[], Point<DIMENSIONS> res) {
		if (d >= n) { //stop clause
			cout << this->operator[](res) << " ";
			// Here we do the work!
			//print(res, n);
			return;
		}
		for (int i = 0; i < size[d]; i++) {
			res.coordinates[d] = i;
			iterate(d + 1, n, size, res);
		}
	}

	friend std::ostream& operator<<(std::ostream& out, const Matrix& m) {

		MatrixPrinter<T, DIMENSIONS>::print(m._array.get(), m._size, m._dimensions, out);

		return out;

	}

	template<class GroupingFunc>
	auto groupValues(GroupingFunc groupingFunc) {
		//using T = deref_iter_t<Iterator>;
		using GroupingType = std::result_of_t<GroupingFunc(T&)>;
		std::map<GroupingType, std::list<T>> groups;

		// Iterate matrix
		// TODO: Create iterator
		Point<DIMENSIONS> point;
		
		iterate(0, DIMENSIONS, _dimensions, point);



		/*
		while (1)
		{
			// Print
			for (int i = 0; i < DIMENSIONS; i++)
			{
				//std::cout << this->operator[](point) << " ";
			}
			std::cout << "\n";

			// Update
			int j;
			for (j = 0; j < DIMENSIONS; j++)
			{	
				
				point.coordinates[j]++;
				if (point.coordinates[j] < _dimensions[j]) break;
				point.coordinates[j] = 0;
			}
			if (j == DIMENSIONS) break;
		}
		*/
		/*
		std::for_each(begin, end, [&groups, groupingFunc](const auto& val) {
			groups[groupingFunc(val)].push_back(val);
		});
		*/
		return groups;
	}

};


// defining Matrix2d<T> as Matrix<T, 2>

template<class T>

using Matrix2d = Matrix<T, 2>;



// defining Matrix3d<T> as Matrix<T, 3>

template<class T>

using Matrix3d = Matrix<T, 3>;


// defining IntVector as Matrix<int, 1>

using IntVector = Matrix<int, 1>;


