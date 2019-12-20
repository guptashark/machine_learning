#include "model_selection.h"


using std::tuple;
using std::vector;
using array_like = vector<vector<double> >;

tuple<array_like, vector<double>, array_like, vector<double>>
model_selection::train_test_split
(array_like X, vector<double> y) {

	// for now, lets just split 50 50.
	// assume that there are any number of vars,
	// but that there are 4 observations/data points.
	
	array_like train_X;
	array_like test_X;

	vector<double> train_y;
	vector<double> test_y;

	train_X.push_back(X[0]);
	train_X.push_back(X[1]);

	test_X.push_back(X[2]);
	test_X.push_back(X[3]);

	train_y.push_back(y[0]);
	train_y.push_back(y[1]);

	test_y.push_back(y[2]);
	test_y.push_back(y[3]);

	return std::make_tuple(train_X, train_y, test_X, test_y);
}
