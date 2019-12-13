#include <vector>
#include <optional>

namespace stat_util_experimental {

	using array_like = std::vector<std::vector<double> >;

	// while pythons numpy.mean will flatten the 2darray
	// if it is 2d, for now, lets instead do something
	// a little more restrictive. If mean is given a
	// 2d array, it also will get an axis, and if none
	// are provided, then it's an error. Mean will also
	// take a 1d array and operate as expected.

	struct mean_args_A {
		const array_like &a;
		std::optional<int> axis = std::nullopt;
	};

	struct mean_args_C {
		const std::vector<double> &a;
	};

	double
	mean ( struct mean_args_C ma);

	std::vector<double>
	mean(struct mean_args_A ma);

	double
	var ( struct mean_args_C ma);

	std::vector<double>
	var(struct mean_args_A ma);

	std::vector<std::vector<double> >
	cov(struct mean_args_A ma);
}

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
