#include <iostream>
#include <fstream>
#include <vector>

#include "matrix.h"
#include "linear_model.h"
#include "stat_util.h"

int main(int argc, char *argv[]) {

	(void)argc;
       	(void)argv;

	{
		using stat_util_experimental::mean;

		std::vector<std::vector<double> > X_2d = {
			{1, 2}, {3, 4}
		};

		std::vector<double> X_1d = { 5, 6, 7, 8};

		auto res_01 = mean({X_2d, 0});
		std::cout << res_01.size() << std::endl;
		std::cout << res_01[0] << " " << res_01[1] << std::endl;

		auto res_02 = mean({.a=X_2d, .axis=1});
		std::cout << res_02.size() << std::endl;
		std::cout << res_02[0] << " " << res_02[1] << std::endl;
	}

	{
		std::vector<std::vector<double> > X = {
			{1., 2., 3., 4., 5.}
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

	std::cout << "Machine Learning" << std::endl;
	return 0;
}
