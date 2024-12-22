#pragma once
#include "config.h"
#include "TransferData.h"


/**
 * @class StopCriterion
 * @brief A class to define a stopping criterion based on the maximum number of iterations.
 *
 * This class provides a mechanism to determine whether an iterative process should stop
 * based on a predefined maximum number of iterations.
 *
 */
class StopCriterion
{
protected:
	size_t n_max;
public:
	StopCriterion(size_t _n_max) : n_max(_n_max) {};

	virtual bool check(TransferData& data) const {
		size_t iter = *data.getCurrIter();
		if (iter > n_max) return false;
		return true;
	};

	virtual string getName() {
		return "max iteration";
	}
};

