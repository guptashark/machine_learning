#include <string>
#include <iostream>

#include "matrix.h"

// we always assume that each vector passed in is a row.

Matrix::Matrix(std::size_t rows, std::size_t cols) {

	for (std::size_t i = 0; i < rows; i++) {
		elements.push_back(std::vector<double>(cols, 0.0));
	}
}


using array_like = std::vector<std::vector<double> >;

// TODO
// initialize off of a 2d array, make our own copy.
// Later, we can probably do a move ctor, with less copying
// and the like. Right now, just make sure this works.
// TODO: Bounds checking... can't take any old 2d array.
Matrix::Matrix(const array_like & arr_2d): elements(arr_2d) {}

Matrix::Matrix(std::initializer_list<std::vector<double>> init) {

	auto it = init.begin();
	elements.push_back(*it);
	std::string err = "Error! row length mismatch. Bad Matrix.";

	std::size_t len = elements[0].size();

	it++;

	while ( it != init.end() ) {

		elements.push_back(*it);

		if ( elements [ elements.size() - 1 ].size() != len ) {
			std::cout << err << std::endl;
		}

		it++;
	}
}

Matrix::Matrix(const Matrix& B) : elements(B.elements) {}

void Matrix::print() {
	// should use some basic setw and lengths...
	for ( auto v : elements ) {
		for ( auto flt : v )
			std::cout << flt << " ";
		std::cout << std::endl;
	}
}

std::size_t Matrix::num_rows(void) const {
	return elements.size();
}

std::size_t Matrix::num_cols(void) const {
	return elements[0].size();
}

Matrix operator+(const Matrix &A, const Matrix& B) {

	std::size_t r = A.num_rows();
	std::size_t c = A.num_cols();

	if ( r != B.num_rows() ) {
		std::cout << "Row value mismatch" << std::endl;
		// need to throw.
	} else if ( c != B.num_cols() ) {
		std::cout << "Col value mismatch" << std::endl;
		// need to throw.
	}

	Matrix C ( r , c );

	for ( std::size_t i = 0; i < r; i++) {
		for(std::size_t j = 0; j < c; j++) {
			C[i][j] = A[i][j] + B[i][j];
		}
	}

	return C;
}

Matrix operator * (const Matrix& A, const Matrix &B) {

	std::size_t r = A.num_rows();
	std::size_t c = B.num_cols();
	std::size_t n = A.num_cols();

	Matrix ret(r, c);

	// now calc the values;
	for ( std::size_t i = 0; i < r; i++) {
		for ( std::size_t j = 0; j < c; j++ ) {
			double s = 0.0;

			for ( std::size_t k = 0; k < n; k++) {
				s += A[i][k] * B[k][j];
			}

			ret[i][j] = s;
		}
	}

	return ret;
}

Matrix operator*(const Matrix& A, const double d) {

	Matrix ret(A.num_rows(), A.num_cols());

	for (std::size_t i = 0; i < A.num_rows(); i++) {
		for (std::size_t j = 0; j < A.num_cols(); j++) {
			ret[i][j] = A[i][j] * d;
		}
	}

	return ret;
}

Matrix operator*(const double d, const Matrix& A) {
	return A * d;
}

Matrix Matrix::inverse(void) const {
	// right now, only support 2x2 matrix.
	std::size_t r = num_rows();
	std::size_t c = num_cols();

	// added support for a "1x1" matrix;
	if (r == 1 && c == 1 ) {
		Matrix ret ( 1, 1);
		ret[0][0] = 1.0 / elements[0][0];
		return ret;
	}

	if( r != 2 || c != 2) {
		std::string err = "Error! only 2x2 inversion supported";
		std::cout << err << std::endl;

		// need to do something... like return,
		// or throw an exception here.
	}

	Matrix ret(2, 2);

	double determinant = 0.0;

	// set the determinant.
	{
		double a = elements[0][0];
		double d = elements[1][1];
		double b = elements[0][1];
		double c = elements[1][0];

		determinant = a * d - b * c;
	}

	ret[0][0] = elements[1][1];
	ret[1][1] = elements[0][0];
	ret[1][0] = -1 * elements[1][0];
	ret[0][1] = -1 * elements[0][1];

	return (1.0 / determinant) * ret;
}

Matrix Matrix::transpose(void) const {

	Matrix ret(num_cols(), num_rows());

	for (std::size_t i = 0; i < num_rows(); i++) {
		for(std::size_t j = 0; j < num_cols(); j++) {
			ret[j][i] = (*this)[i][j];
		}
	}

	return ret;
}

std::vector<double>&
Matrix::operator[](std::size_t row) {
	return elements[row];
}

const std::vector<double>&
Matrix::operator[](std::size_t row) const {
	return elements[row];
}
