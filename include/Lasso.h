#include "predictor.h"

namespace linear_model {

	struct Lasso : public Predictor {

		// if it's 0.0, then we have simple LinearRegression.
		double alpha = 1.0;

		// calculate intercept, or set to be 0.
		// (true means calc)
		bool fit_intercept = true;

		// these should not be set, they're what we're computing.
		std::vector<double> coef_ ;
		double intercept_ ;

		// the methods themselves.
		Lasso & fit
		(const array_like &X, const std::vector<double> &y);

		std::vector<double> predict(const array_like & X);

		double score
		(const array_like &X, const std::vector<double> &y);
	};
}
