#include <cmath>
#include <vector>

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

namespace my_util {

	using array_like = std::vector<std::vector<double> >;

	// i is the index of the column.
	// is this overload necessary?
	double
	my_mean
	(const array_like &X, std::size_t i) {
		double s = 0.0;
		for (auto i : X[i]) s += i;
		return s / X[i].size();
	}

	double
	my_mean
	(const std::vector<double>& v) {
		double s = 0.0;
		for(auto i : v) s += i;
		return s / v.size();
	}

	double
	my_variance
	(const std::vector<double>& v, double v_mean) {
		double s = 0.0;
		for ( auto i : v ) s+= ( i - v_mean ) * ( i - v_mean);
		return s / v.size();
	}
	double
	my_variance
	(const std::vector<double>& v) {
		double v_mean = my_mean(v);
		return my_variance(v, v_mean);
	}

	// i and j are the indices of the variable that
	// that hold the cols of the variables we want
	// the covariance of.
	double
	my_cov
	(const array_like &X, std::size_t i, std::size_t j) {

		const std::vector<double>& v1 = X[i];
		const std::vector<double>& v2 = X[j];
		double v1_mean = my_mean(X, i);
		double v2_mean = my_mean(X, j);
		double s = 0.0;

		for (std::size_t k = 0; i < v1.size(); k++) {
			s += (v1[k] - v1_mean) * (v2[k] - v2_mean);
		}

		return s / X[i].size();
	}

	// overload
	double my_cov
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
	my_cov
	(const std::vector<double>& x1, const std::vector<double>& x2) {

		double x1_mean = my_mean(x1);
		double x2_mean = my_mean(x2);

		return my_cov(x1, x2, x1_mean, x2_mean);
	}
}

using linear_model::LinearRegression;
using namespace my_util;

// TODO: properly handle the case where there is
// a vector of vectors, each with length 1. (python style)
LinearRegression &LinearRegression::fit
(const array_like &X, const std::vector<double> &y) {

	if ( X.size() > 2 ) {
		std::cout << "Error: too many variables!" << std::endl;
		return *this;
	}

	const std::size_t n = X.size();
	const double y_mean = my_mean(y);
	std::vector<double> x_means; // mean of each column.
	std::vector<double> x_y_cov; // covariances of each x col with y.

	Matrix A ( n, n ); // store x_covariances in matrix

	// separate loops to avoid recalculating means to find covariances.
	for(std::size_t i = 0; i < n; i++) {
		x_means.push_back( my_mean(X[i]) );
	}

	for(std::size_t i = 0; i < n; i++) {
		x_y_cov.push_back( my_cov(X[i], y, x_means[i], y_mean) );
	}

	for ( std::size_t i = 0; i < n; i++) {
		A[i][i] = my_variance( X[i], x_means[i] );
	}

	for ( std::size_t i = 0; i < n; i++) {
		for ( std::size_t j = i + 1 ; j < n; j++) {

			double r = my_cov(X[i], X[j], x_means[i], x_means[j]);

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

	return 1.0 - u / (y.size() * my_variance(y));
}
