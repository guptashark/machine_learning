#include <vector>

#include "stat_util.h"

namespace stat_util_experimental {

	using array_like = std::vector<std::vector<double> >;

	double
	mean ( struct mean_args_C ma) {

		double sum = 0;
		for ( auto val : ma.a ) {
			sum += val;
		}

		return sum / ma.a.size();
	}

	// currently only supports axis 0 and 1.
	// TODO actually throw exceptions
	// TODO put the computes in another fn, to avoid
	// nesting too deep. Also makes sense, since the
	// logic seems fairly similar.
	std::vector<double>
	mean(struct mean_args_A ma) {

		if ( ! ma.axis.has_value() ) {
			// TODO can't compute, need to throw excp.
			return {};
		}

		std::vector<double> averages;

		// row based;
		if ( *(ma.axis) == 1) {
			for( const std::vector<double> & row : ma.a ) {
				averages.push_back(mean({row}));
			}
		} else if ( *(ma.axis) == 0) {

			const std::size_t m = ma.a.size();
			const std::size_t n = ma.a.front().size();

			for(std::size_t j = 0; j < n; j++) {
				double col_mean = 0;

				for(std::size_t i = 0; i < m; i++) {
					col_mean += ma.a[i][j];
				}

				averages.push_back(col_mean / n);
			}
		} else {
			// throw an exception.
			return {};
		}

		return averages;
	}

	double
	var ( struct mean_args_C ma) {

		// TODO - optimize this, so we don't recompute means.
		// inefficient for now, but it's okay, optimize later.

		double x_mean = mean( ma );

		double sum = 0;
		for ( auto val : ma.a ) {
			sum += (val - x_mean) * (val - x_mean);
		}

		return sum / ma.a.size();
	}

	std::vector<double>
	var(struct mean_args_A ma) {

		if ( ! ma.axis.has_value() ) {
			// TODO can't compute, need to throw excp.
			return {};
		}

		std::vector<double> variances;

		// row based;
		if ( *(ma.axis) == 1) {
			for( const std::vector<double> & row : ma.a ) {
				variances.push_back( var ( {row} ));
			}
		} else if ( *(ma.axis) == 0) {

			// this notation makes it easier
			std::vector<double> means = mean( {ma.a, ma.axis} );

			const std::size_t m = ma.a.size();
			const std::size_t n = ma.a.front().size();

			for(std::size_t j = 0; j < n; j++) {

				double x_mean = means[j];

				// short for col_variance
				double col_var = 0;

				for(std::size_t i = 0; i < m; i++) {
					double term = ma.a[i][j] - x_mean;
					col_var += term * term;
				}

				variances.push_back(col_var / n);
			}
		} else {
			// throw an exception.
			return {};
		}

		return variances;
	}

	std::vector<std::vector<double> >
	cov(struct mean_args_A ma) {

		const std::size_t n = ma.a.size();
		array_like covs(n, std::vector<double>(n));
		std::vector<double> means = mean( {ma.a, .axis=1} );
		const array_like &A = ma.a;

		// first do the diagonal... which has to be 1?
		for(std::size_t i = 0; i < n; i++) {

			double c = 0.0;
			const std::vector<double> &v = A[i];

			for(std::size_t j = 0; j < v.size(); j++) {
				c += (v[j] - means[i]) * (v[j] - means[i]);
			}

			covs[i][i] = c / n;
		}

		for(std::size_t i = 0; i < n; i++) {

			for(std::size_t j = i+1; j < n; j++) {

				double c = 0.0;
				const std::vector<double> &v = A[i];
				const std::vector<double> &w = A[j];

				for(std::size_t k = 0; k < v.size(); k++) {
					double v_term = v[k] - means[i];
					double w_term = w[k] - means[j];
					c += v_term * w_term;
				}

				covs[i][j] = c/n;
				covs[j][i] = c/n;
			}
		}

		return covs;
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
