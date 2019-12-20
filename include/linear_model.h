#include <vector>

struct Predictor {

	//using array_like = std::vector<std::vector<double>>;
	using array_like = std::vector<std::vector<double> >;

	virtual Predictor &fit
	(const array_like &X,
	 const std::vector<double> &y) = 0;

	virtual double score
	(const array_like &X,
	 const std::vector<double> &y) = 0;

	virtual ~Predictor() {};
};

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
