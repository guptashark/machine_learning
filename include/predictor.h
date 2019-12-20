#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <vector>

struct Predictor {

	using array_like = std::vector<std::vector<double> >;

	virtual Predictor &fit
	(const array_like &X,
	 const std::vector<double> &y) = 0;

	virtual double score
	(const array_like &X,
	 const std::vector<double> &y) = 0;

	virtual ~Predictor() {};
};

#endif
