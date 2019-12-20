#include "predictor.h"

namespace linear_model {

	using array_like = std::vector<std::vector<double> >;

	struct LinearRegression : public Predictor {

		bool fit_intercept = true;
		bool normalize = false;
		bool copy_X = true;
		int n_jobs = 0;

		// these shouldn't be set, since they'll be overridden.
		std::vector<double> coef_ = {};
		double intercept_ = 0.0;

		LinearRegression &fit
		(const array_like &X,
		 const std::vector<double> &y);

		std::vector<double> predict
		(const array_like &X);

		double score
		(const array_like &X,
		 const std::vector<double> &y);
	};
}
