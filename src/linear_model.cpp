#include <cmath>
#include <vector>

#include "stat_util.h"
#include "matrix.h"
#include "linear_model.h"

// NOTE: std::vector<std::vector<double> >
// is a vector of vectors. Each of the internal vectors
// are columns, vs each of the internal vectors being
// a row in our dataset. This (I think) makes much more
// sense, because we can run analyses on different
// columns, and it is is often the case that we want to
// pick particular features, which is much easier to do
// when each internal vector is an entire column.

using linear_model::LinearRegression;

// TODO: properly handle the case where there is
// a vector of vectors, each with length 1. (python style)
LinearRegression &LinearRegression::fit
(const array_like &X, const std::vector<double> &y) {

	// best option - make a copy of X, call it XT (X transposed)
	array_like XT;
	for ( std::size_t i = 0; i < X.front().size(); i++) {
		XT.push_back(std::vector<double>(X.size()));
	}

	for(std::size_t i = 0; i < X.size(); i++) {
		for ( std::size_t j = 0; j < X.front().size(); j++) {
			XT[j][i] = X[i][j];
		}
	}

	using stat_util_experimental::mean;
	using stat_util::cov;

	if ( XT.size() > 2 ) {
		std::cout << "Error: too many variables!" << std::endl;
		return *this;
	}

	const std::size_t n = XT.size();
	const double y_mean = mean({y});
	std::vector<double> x_means = mean({ XT, .axis=1});
	std::vector<double> x_y_cov; // covariances of each x col with y.

	for(std::size_t i = 0; i < n; i++) {
		x_y_cov.push_back( cov(XT[i], y, x_means[i], y_mean) );
	}

	// bias bc we want to divide by N, not N-1
	Matrix A ( stat_util_experimental::cov({XT, .bias=true}) );

	// Solving Ax = b
	Matrix B ( n, 1);

	for (std::size_t i = 0; i < n ; i++) {
		B[i][0] = x_y_cov[i];
	}

	Matrix sln = A.inverse() * B;

	// clear the coef_ in case it was set before.
	coef_.clear();

	for (std::size_t i = 0; i < n ; i++) {
		coef_.push_back( sln[i][0] );
	}

	intercept_ = y_mean;

	for(std::size_t i = 0; i < n ; i++) {
		intercept_ += (-1.0) * coef_[i] * x_means[i];
	}

	return *this;
}

std::vector<double> LinearRegression::predict
(const array_like &X) {
	std::vector<double> predictions;

	std::size_t num_rows = X[0].size();

	for (std::size_t i = 0; i < num_rows; i++) {

		double r = 0.0;

		for (std::size_t j = 0; j < X.size(); j++) {
			r += X[j][i] * coef_.at(j);
		}

		r += intercept_ ;
		predictions.push_back(r);
	}

	return predictions;
}

double linear_model::LinearRegression::score
(const array_like &X, const std::vector<double> &y) {

	// we are using the formula: r^2 = 1 - u/v where,
	// u = residual sum of squares = sum ((y_true - y_pred) ** 2)
	// v = sum((y_true - y_mean) ** 2) = n * variance(y)

	double u = 0.0;

	for (std::size_t i = 0; i < y.size() ; i++) {

		double y_pred_i = intercept_;

		for (std::size_t j = 0; j < coef_.size(); j++) {
			y_pred_i += coef_[j] * X[i][j];
		}

		u += std::pow(y[i] - y_pred_i, 2) ;
	}

	return 1.0 - u / (y.size() * stat_util::var(y));
}
