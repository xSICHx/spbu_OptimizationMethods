#pragma once
#include "config.h"


/**
 * @class Function
 * @brief Abstract base class representing a mathematical function.
 *
 * This class provides an interface for mathematical functions, including methods
 * to get the function's dimension, name, gradient, and Hessian matrix.
 */
class Function
{
private:
	double h = 1e-5;

public:
	virtual ~Function() = default;

	virtual size_t getDim() const = 0;
	virtual string getName() const = 0;

	virtual point_type operator()(const VectorXd& point) const = 0;

	

	VectorXd getGradient(const VectorXd& point) const;
	MatrixXd getHessianMatrix(const VectorXd& point) const;
};

