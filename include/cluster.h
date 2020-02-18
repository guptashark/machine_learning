#include <optional>

namespace cluster {

	struct KMeans {

		using array_like = std::vector<std::vector<double> >;

		int n_clusters = 8;
		int n_init = 10;
		int max_iter = 300;
		double tol = 0.0001;
		
		// set to have deterministic centroid initialization.
		std::optional<int> random_state = std::nullopt;

		// the output members of our algos.
		// cluster_centers (likely array_like)
		// TODO Labels???
		// labels_ ???
		double inertia_ ;
		int n_iter_ ;

		

		// Member functions.
		KMeans &fit
		(const array_like &X,
		 const std::vector<double> &y);

		// TODO
		// What is the return type here? What are labels?
		void fit_predict
		(const array_like &X,
		 const std::vector<double> &y);

		std::vector<double> predict
		(const array_like &X);

		double score
		(const array_like &X,
		 const std::vector<double> &y);
	};
}
