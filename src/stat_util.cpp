#include <vector>

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
