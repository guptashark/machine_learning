#include <vector>

#include "stat_util.h"


using array_like = std::vector<std::vector<double> >;

double
stat_util_experimental::mean ( struct mean_args_C ma) {

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
stat_util_experimental::mean(struct mean_args_A ma) {

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

			averages.push_back(col_mean / m);
		}
	} else {
		// throw an exception.
		return {};
	}

	return averages;
}

double
stat_util_experimental::var ( struct mean_args_C ma) {

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
stat_util_experimental::var(struct mean_args_A ma) {

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
stat_util_experimental::cov(struct cov_args ma) {

	// columns are variables (as standard python)
	// rows are observations.
	// number of columns is the size of the output covs matrix.
	// m is the number of rows.
	// n is the number of columns.
	// we have a vector of row vectors.
	if ( ma.rowvar == false ) {
		std::size_t m = ma.a.size();
		const std::size_t n = ma.a[0].size();

		array_like covs(n, std::vector<double>(n));
		// axis is 0, means take averages down a column.
		// is this even implemented yet??
		std::vector<double> means = mean( {ma.a, .axis=0} );

		const array_like & A = ma.a;

		// first do the diagonal
		for(std::size_t i = 0; i < m; i++) {

			for ( std::size_t j = 0; j < n; j++) {
				double term = A[i][j] - means[j];

				covs[j][j] += term * term;
			}
		}

		for ( std::size_t j = 0; j < n; j++) {
			if ( ma.bias == false ) {
				covs[j][j] /= (m - 1);
			} else {
				covs[j][j] /= m;
			}
		}

		// now do the other parts, the middles.
		for(std::size_t i = 0; i < n; i++) {
			for(std::size_t j = i + 1; j < n; j++) {

				double c = 0.0;

				for(std::size_t k = 0; k < m; k++) {

					double i_term = A[k][i]-means[i];
					double j_term = A[k][j]-means[j];

					c += i_term * j_term;
				}

				if ( ma.bias == false) {
					covs[i][j] = c / (m - 1);
					covs[j][i] = c / (m - 1);
				} else {
					covs[i][j] = c / m;
					covs[j][i] = c / m;
				}
			}
		}

		return covs;
	}


	// the else case - rowvar is true (default)

	const std::size_t n = ma.a.size(); 	// rows
	std::size_t m = ma.a[0].size(); 	// columns

	// bias is guaranteed to have a value.
	if ( ! ma.bias ) {
		m = m - 1;
	}

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

		covs[i][i] = c / m;
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

			covs[i][j] = c/m;
			covs[j][i] = c/m;
		}
	}

	return covs;
}


// i is the index of the column.
// is this overload necessary?
double
stat_util::mean
(const array_like &X, std::size_t i) {
	double s = 0.0;
	for (auto i : X[i]) s += i;
	return s / X[i].size();
}

double
stat_util::mean
(const std::vector<double>& v) {
	double s = 0.0;
	for(auto i : v) s += i;
	return s / v.size();
}

double
stat_util::var
(const std::vector<double>& v, double v_mean) {
	double s = 0.0;
	for ( auto i : v ) s+= ( i - v_mean ) * ( i - v_mean);
	return s / v.size();
}

double
stat_util::var
(const std::vector<double>& v) {
	double v_mean = mean(v);
	return var(v, v_mean);
}

// i and j are the indices of the variable that
// that hold the cols of the variables we want
// the covariance of.
double
stat_util::cov
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
double
stat_util::cov
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
stat_util::cov
(const std::vector<double>& x1, const std::vector<double>& x2) {

	double x1_mean = mean(x1);
	double x2_mean = mean(x2);

	return cov(x1, x2, x1_mean, x2_mean);
}
