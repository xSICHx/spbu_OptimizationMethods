#pragma once
#include "config.h"


class Function
{
private:
	double h = 1e-3;

public:
	virtual ~Function() = default;

	virtual size_t getDim() const = 0;

	virtual point_type operator()(const VectorXd& point) const = 0;

	

	VectorXd getGradient(const VectorXd& point) const;
	MatrixXd getGoesseMatrix(const VectorXd& point) const;
};

