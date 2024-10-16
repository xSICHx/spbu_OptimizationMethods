#pragma once
#include "config.h"


class Area
{
private:
	// matrix(n, 2). ѕо первому индексу - верхн€€, нижн€€ границы
	MatrixXd bounds;

public:
	Area(const MatrixXd& _bounds) {
		bounds = _bounds;
	}

	MatrixXd GetBounds() const {
		return bounds;
	}

	double getILeftBound(size_t i) const {
		return bounds(i, 0);
	}
	double getIRightBound(size_t i) const {
		return bounds(i, 1);
	}
	int getAreaDim() const {
		return bounds.rows();
	}
};

