#include <cassert>
#include <vector>

#include "matrix.h"
#include "linear_model.h"
#include "stat_util.h"
#include "tests.h"

using array_like = std::vector<std::vector<double> >;

static void test_mean(void);
static void test_var(void);
static void test_cov(void);

void test_stat_util(void) {

	// TODO : check this out.
	// These assertions in these fn's work bc
	// none of the computations are ever
	// dealing with repeating decimals,
	// so we never encounter any inaccuracies.
	// ie, not doing (1.0 / 3.0) * 3.0.
	//
	// However, some inaccurances still slip
	// through, and the assertions fail. It's
	// also a bad idea to rely on well behaved
	// floating point behavior so we need to
	// do better.

	test_mean();
	test_var();
	test_cov();
}

void test_linear_model(void) {

	{
		std::vector<std::vector<double> > X = {
			{1.}, {2.}, {3.}, {4.}, {5.}
		};

		std::vector<double> y;
		for ( std::vector<double> & r : X ) {
			y.push_back(r[0] * 2.0 + 4.0);
		}

		auto reg = linear_model::LinearRegression{}.fit(X, y);
		assert( 2.0 == reg.coef_[0] );
		assert( 4.0 == reg.intercept_ );
		assert( 1.0 == reg.score(X, y) );

		std::vector<std::vector<double> > X_test = {
			{ -9.0, -2.0, 0.0, 1000.0 }
		};

		std::vector<double> p = reg.predict(X_test);
		std::vector<double> y_test = { -14.0, 0.0, 4.0, 2004.0};

		assert(p == y_test);
	}

	// TODO : Make this stuff a lot more robust.
	// Do some floating point tests with not nice numbers,
	// without having to print to file.
	{
		std::vector<std::vector<double> > X = {
			{1, 4},
			{1, 7},
			{2, 6},
			{3, 11},
		};

		std::vector<double> y;

		for( std::vector<double> & r : X ) {
			y.push_back(4.0 * r[0] - 2.0 * r[1] -9.0);
		}

		auto reg = linear_model::LinearRegression{}.fit(X, y);

		assert( 4.0 == reg.coef_[0] );
		assert( -2.0 == reg.coef_[1] );
		assert( -9.0 == reg.intercept_ );
		assert( 1.0 == reg.score(X, y));
	}
}

static void test_mean(void) {
	using stat_util_experimental::mean;

	std::vector<std::vector<double> > X = {
		{1, 2},
		{3, 4}
	};

	{
		auto res = mean({X, 0});
		assert(res.size() == 2);
		assert(res[0] == 2);
		assert(res[1] == 3);
	}

	{
		auto res = mean({.a=X, .axis=1});
		assert(res.size() == 2);
		assert(res[0] == 1.5);
		assert(res[1] == 3.5);
	}
}

static void test_var(void) {
	using stat_util_experimental::var;

	std::vector<std::vector<double> > X = {
		{1, 2},
		{3, 4}
	};

	// TODO : check this out.
	// These assertions will work because
	// none of the computations are ever
	// dealing with repeating decimals,
	// so we never encounter any inaccuracies.
	// ie, not doing (1.0 / 3.0) * 3.0.
	{
		auto res = var({X, 0});
		assert(res.size() == 2);
		assert(res[0] == 1);
		assert(res[1] == 1);
	}

	{
		auto res = var({X, 1});
		assert(res.size() == 2);
		assert(res[0] == 0.25);
		assert(res[1] == 0.25);
	}
}

static void test_cov(void) {

	using stat_util_experimental::cov;

	// while we're figuring it out, we'll only
	// do tests with nice round whole numbers, that still
	// guarantee that the function computation is correct.

	{
		std::vector<std::vector<double> > X = {
			{0, 1, 2},
			{2, 1, 0},
		};

		array_like ans = {{1, -1}, {-1, 1}};
		assert(cov( {X} ) == ans );
	}

	{
		std::vector<std::vector<double> > X = {
			{0, 2},
			{1, 1},
			{2, 0}
		};

		array_like ans = {{1, -1}, {-1, 1}};
		assert(cov( {X, .rowvar=false} ) == ans );
	}
}

void test_matrix(void) {

	// do nothing, not yet written good tests.
}
