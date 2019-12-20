#include <cmath>
#include <vector>

#include "LinearRegression.h"

#include "stat_util.h"
#include "matrix.h"

// NOTE: std::vector<std::vector<double> >
// is a vector of vectors. Each of the internal vectors
// are columns, vs each of the internal vectors being
// a row in our dataset. This (I think) makes much more
// sense, because we can run analyses on different
// columns, and it is is often the case that we want to
// pick particular features, which is much easier to do
// when each internal vector is an entire column.

using linear_model::LinearRegression;

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
	using stat_util_experimental::cov;

	if ( XT.size() > 2 ) {
		throw "too many variables!";
	}

	const std::size_t n = XT.size();

	XT.push_back(y);

	// bias bc we want to divide by N, not N-1
	array_like covariances = cov({XT, .bias=true});

	// Solving Ax = b
	Matrix B ( n, 1);

	for (std::size_t i = 0; i < n ; i++) {
		B[i][0] = covariances.back()[i];
	}

	covariances.pop_back();

	for( std::vector<double> & row : covariances ) {
		row.pop_back();
	}

	Matrix A ( covariances );

	Matrix sln = A.inverse() * B;

	// clear the coef_ in case it was set before.
	coef_.clear();

	for (std::size_t i = 0; i < n ; i++) {
		coef_.push_back( sln[i][0] );
	}

	intercept_ = mean({y});

	std::vector<double> x_means = mean({ XT, .axis=1});

	for(std::size_t i = 0; i < n ; i++) {
		intercept_ += (-1.0) * coef_[i] * x_means[i];
	}

	return *this;
}

std::vector<double> LinearRegression::predict
(const array_like &X) {

	std::vector<double> predictions;

	for (const std::vector<double> & row : X) {

		double r = intercept_ ;

		for (std::size_t j = 0; j < row.size(); j++) {
			r += row[j] * coef_.at(j);
		}

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
	std::vector<double> predictions = predict(X);

	for (std::size_t i = 0; i < y.size() ; i++) {
		u += std::pow(y[i] - predictions[i], 2) ;
	}

	return 1.0 - u / (y.size() * stat_util_experimental::var({y}));
}
