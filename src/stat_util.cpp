#include <vector>

#include "stat_util.h"

namespace stat_util_experimental {

	using array_like = std::vector<std::vector<double> >;

	// currently only supports axis 0 and 1.
	std::vector<double>
	mean(struct mean_args_A ma) {

		if ( ! ma.axis.has_value() ) {
			// TODO can't compute, need to throw excp.
			return {};
		}

		// can compute.

		std::vector<double> averages;

		// row based;
		if ( *(ma.axis) == 1) {
			for( const std::vector<double> & row : ma.a ) {
				averages.push_back(mean({row}));
			}
		} else if ( *(ma.axis) == 0) {
			for(std::size_t j = 0; j < ma.a[0].size(); j++) {
				double col_mean = 0;

				for(std::size_t i = 0; i < ma.a.size(); i++) {
					col_mean += ma.a[i][j];
				}

				averages.push_back(col_mean / ma.a[0].size());
			}
		} else {
			// throw an exception.
			return {};
		}

		return averages;
	}

	double
	mean ( struct mean_args_C ma) {

		double sum = 0;
		for ( auto val : ma.a ) {
			sum += val;
		}

		return sum / ma.a.size();
	}
}

namespace stat_util {

	using array_like = std::vector<std::vector<double> >;

	// i is the index of the column.
	// is this overload necessary?
	double
	mean
	(const array_like &X, std::size_t i) {
		double s = 0.0;
		for (auto i : X[i]) s += i;
		return s / X[i].size();
	}

	double
	mean
	(const std::vector<double>& v) {
		double s = 0.0;
		for(auto i : v) s += i;
		return s / v.size();
	}

	double
	var
	(const std::vector<double>& v, double v_mean) {
		double s = 0.0;
		for ( auto i : v ) s+= ( i - v_mean ) * ( i - v_mean);
		return s / v.size();
	}

	double
	var
	(const std::vector<double>& v) {
		double v_mean = mean(v);
		return var(v, v_mean);
	}

	// i and j are the indices of the variable that
	// that hold the cols of the variables we want
	// the covariance of.
	double
	cov
	(const array_like &X, std::size_t i, std::size_t j) {

		const std::vector<double>& v1 = X[i];
		const std::vector<double>& v2 = X[j];
		double v1_mean = mean(X, i);
		double v2_mean = mean(X, j);
		double s = 0.0;

		for (std::size_t k = 0; i < v1.size(); k++) {
			s += (v1[k] - v1_mean) * (v2[k] - v2_mean);
		}

		return s / X[i].size();
	}

	// overload
	double cov
	(const std::vector<double>& x1, const std::vector<double>& x2,
	 double x1_mean, double x2_mean)
	{
		double s = 0.0;

		for(std::size_t i = 0; i < x1.size(); i++) {
			s += (x1[i] - x1_mean) * (x2[i] - x2_mean);
		}

		return s / x1.size();
	}

	// overload
	double
	cov
	(const std::vector<double>& x1, const std::vector<double>& x2) {

		double x1_mean = mean(x1);
		double x2_mean = mean(x2);

		return cov(x1, x2, x1_mean, x2_mean);
	}
}
