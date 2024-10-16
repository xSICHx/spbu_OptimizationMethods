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

	double getILeftBound(const size_t i) const {
		return bounds(i, 0);
	}
	double getIRightBound(const size_t i) const {
		return bounds(i, 1);
	}
	int getAreaDim() const {
		return bounds.rows();
	}
	// true if in area
	bool checkPointInArea(const VectorXd& point) const{
		bool flag = 1;
		for (size_t i = 0; i < getAreaDim(); ++i) {
			flag *= bounds(i, 0) < point[i] && point[i] < bounds(i, 1);
		}
		return flag;
	}
};

