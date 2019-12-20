#include <vector>
#include <tuple>


namespace model_selection {

	using std::tuple;
	using std::vector;

	using array_like = vector<vector<double> >;

	tuple<array_like, vector<double>, array_like, vector<double>>
	train_test_split
	(array_like X, std::vector<double> y);
}
