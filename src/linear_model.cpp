#include <cmath>
#include <vector>

#include "stat_util.h"
#include "matrix.h"
#include "linear_model.h"

// we'll put this (the util stuff) somewhere else,
// but for now, lets have it here

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

	using stat_util::mean;
	using stat_util::var;
	using stat_util::cov;

	if ( X.size() > 2 ) {
		std::cout << "Error: too many variables!" << std::endl;
		return *this;
	}

	const std::size_t n = X.size();
	const double y_mean = mean(y);
	std::vector<double> x_means; // mean of each column.
	std::vector<double> x_y_cov; // covariances of each x col with y.

	Matrix A ( n, n ); // store x_covariances in matrix

	// separate loops to avoid recalculating means to find covariances.
	for(std::size_t i = 0; i < n; i++) {
		x_means.push_back( mean(X[i]) );
	}

	for(std::size_t i = 0; i < n; i++) {
		x_y_cov.push_back( cov(X[i], y, x_means[i], y_mean) );
	}

	for ( std::size_t i = 0; i < n; i++) {
		A[i][i] = var( X[i], x_means[i] );
	}

	for ( std::size_t i = 0; i < n; i++) {
		for ( std::size_t j = i + 1 ; j < n; j++) {

			double r = cov(X[i], X[j], x_means[i], x_means[j] );

			A[i][j] = r;
			A[j][i] = r;
		}
	}

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
			y_pred_i +=  coef_[j] * X[j][i];
		}

		u += std::pow(y[i] - y_pred_i, 2) ;
	}

	return 1.0 - u / (y.size() * stat_util::var(y));
}
