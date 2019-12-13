#include <cassert>
#include <vector>

#include "matrix.h"
#include "linear_model.h"
#include "stat_util.h"
#include "tests.h"

// Helpful for printing stuff!
void v_prnt(const std::vector<double> &v) {

	std::cout << "[";

	for ( std::size_t i = 0; i < v.size() - 1; i++ ) {
		std::cout << v[i] << ", ";
	}

	std::cout << v[v.size() - 1] << "]" << std::endl;
}

void arr_2d_prnt(std::vector<std::vector<double> > & A) {

	std::cout << "Begin matrix:" << std::endl;

	for ( std::vector<double> & v : A ) {
		v_prnt ( v );
	}

	std::cout << "End matrix" << std::endl;
}

static void test_cov(void);

void test_stat_util(void) {

	test_cov();

	using stat_util_experimental::mean;
	using stat_util_experimental::var;

	std::vector<std::vector<double> > X_2d = {
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
		auto res = mean({X_2d, 0});
		assert(res.size() == 2);
		assert(res[0] == 2);
		assert(res[1] == 3);
	}

	{
		auto res = mean({.a=X_2d, .axis=1});
		assert(res.size() == 2);
		assert(res[0] == 1.5);
		assert(res[1] == 3.5);
	}

	{
		auto res = var({X_2d, 0});
		assert(res.size() == 2);
		assert(res[0] == 1);
		assert(res[1] == 1);
	}

	{
		auto res = var({X_2d, 1});
		assert(res.size() == 2);
		assert(res[0] == 0.25);
		assert(res[1] == 0.25);
	}
}

void test_linear_model(void) {

	{
		std::vector<std::vector<double> > X = {
			{1., 2., 3., 4., 5.}
		};

		std::vector<std::vector<double> > X_py_style = {
			{1.}, {2.}, {3.}, {4.}, {5.}
		};

		std::vector<double> y = {
			1.5, 1.7, 3.2, 4.5, 5.19
		};

		auto reg = linear_model::LinearRegression{}.fit(X, y);
		std::cout << "Coef" << std::endl;
		std::cout << reg.coef_[0] << std::endl;
		std::cout << "Intercept" << std::endl;
		std::cout << reg.intercept_ << std::endl;
		std::cout << "Score" << std::endl;
		std::cout << reg.score(X, y) << std::endl;

		std::vector<double> p = reg.predict(X);
		for (auto val : p  ) std::cout << val << " ";
		std::cout << std::endl;
	}

	{
		std::vector<std::vector<double> > X = {
			{0.9, 2.3, 3.1, 3.7, 5.4},
			{1.3, 2.9, 3.7, 4.4, 5.8}
		};

		std::vector<std::vector<double> > X_py_style = {
			{0.9, 1.3},
			{2.3, 2.9},
			{3.1, 3.7},
			{3.7, 4.4},
			{5.4, 5.8}
		};

		std::vector<double> y = {
			5.5, 10.9, 14.0, 22.3, 27.1
		};

		auto reg = linear_model::LinearRegression{}.fit(X, y);

		std::cout << "Coef" << std::endl;
		std::cout << reg.coef_[0] << " " << reg.coef_[1] << std::endl;
		std::cout << "Intercept" << std::endl;
		std::cout << reg.intercept_ << std::endl;
		std::cout << "Score" << std::endl;
		std::cout << reg.score(X, y) << std::endl;

		std::vector<double> p = reg.predict(X);
		for (auto val : p  ) std::cout << val << " ";
		std::cout << std::endl;
	}
}

static void test_cov(void) {

	using stat_util_experimental::cov;

	std::vector<std::vector<double> > X_0 = {
		{1, 2, 3, 4, 5, 6, 7},
		{4, 5, 6, 7, 8, 9, 10},
		{9, 9, 9, 9, 9, 9, 9}
	};

	(void)X_0;

	std::vector<std::vector<double> > X_1 = {
		{0, 1, 2},
		{2, 1, 0},
	};
	auto r = cov ( {X_1} );

	arr_2d_prnt(r);
	{
		std::vector<std::vector<double> > X = {
			{-2.1, -1, 4.3},
			{3, 1.1, 0.12},
		};

		auto r = cov ({X});
		arr_2d_prnt(r);
	}

}
