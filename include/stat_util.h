#include <vector>

namespace stat_util {

	using array_like = std::vector<std::vector<double> >;

	// i is the index of the column.
	// is this overload necessary?
	double
	mean
	(const array_like &X, std::size_t i);

	double
	mean
	(const std::vector<double>& v);

	// get the variance
	double
	var
	(const std::vector<double>& v, double v_mean);

	double
	var
	(const std::vector<double>& v);

	// i and j are the indices of the variable that
	// that hold the cols of the variables we want
	// the covariance of.
	double
	cov
	(const array_like &X, std::size_t i, std::size_t j);

	// overload
	double cov
	(const std::vector<double>& x1, const std::vector<double>& x2,
	 double x1_mean, double x2_mean);

	// overload
	double
	cov
	(const std::vector<double>& x1, const std::vector<double>& x2);
}
