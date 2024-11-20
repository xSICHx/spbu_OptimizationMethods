#pragma once
#include "config.h"
#include "TransferData.h"


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

