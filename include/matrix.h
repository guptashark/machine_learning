#include <vector>
#include <initializer_list>

class Matrix {

private:
	// since we typically have a matrix A.
	std::vector<std::vector<double> > elements;
public:
	// creates a zeroed matrix of this size.
	Matrix(std::size_t rows, std::size_t cols);

	Matrix(const Matrix& B);

	// TODO: many improvements.
	Matrix(const std::vector<std::vector<double> > & arr_2d);

	// each of the vectors need to be the same size.
	// otherwise, will fail, and should... throw?
	Matrix(std::initializer_list<std::vector<double>> init);

	void print(void);

	std::size_t num_rows(void) const;
	std::size_t num_cols(void) const;

	friend Matrix operator+(const Matrix& A, const Matrix& B);
	friend Matrix operator*(const Matrix& A, const Matrix& B);
	friend Matrix operator*(const Matrix& A, const double d);
	friend Matrix operator*(const double d, const Matrix& A);

	friend bool operator==(const Matrix & A, const Matrix & B);

	// these methods create whole new matrices.
	Matrix inverse(void) const;
	Matrix transpose(void) const;

	std::vector<double>& operator[](std::size_t row);
	const std::vector<double>& operator[](std::size_t row) const;
};
