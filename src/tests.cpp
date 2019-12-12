#include <cassert>

#include "matrix.h"
#include "linear_model.h"
#include "stat_util.h"
#include "tests.h"

void test_stat_util(void) {

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
