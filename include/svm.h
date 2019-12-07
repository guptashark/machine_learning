#include <string>

namespace svm {

	struct LinearSVC {

		// Parameters
		std::string penalty;
		std::string loss;
		bool dual;
		double tol;
		double C;
		std::string multi_class;
		bool fit_intercept;
		double intercept_scaling;
		// class_weight  ?? what is this
		int verbose;
		int random_state;
		int max_iter;

		// Attributes: (don't write to these from outside)
		double coef_ ;
		double intercept_ ;




		LinearSVC & fit(const std::vector<double> &X, const std::vector<double> &y) {
			// do a check to ensure args have been set right.
			// overwrites the coef and intercept. 

			(void)X;
			(void)y;

			coef_ = 50.0;
			intercept_ = 100.0;
			
			std::cout << "LinearSVC fit called!" << std::endl;
			return *this;
		}

		double score(const std::vector<double> &X, const std::vector<double> &y) {

			(void)X;
			(void)y;

			std::cout << "LinearSVC score called!" << std::endl;
			return 3.5;
		}

	};	

}
