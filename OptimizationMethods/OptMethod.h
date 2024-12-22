#pragma once
#include "Area.h"
#include "Function.h"
#include "StopCriterion.h"


/**
 * @class OptMethod
 * @brief Abstract base class for optimization methods.
 *
 * This class provides an interface for optimization methods. It declares
 * pure virtual functions that must be implemented by derived classes.
 */
class OptMethod
{

public:
	
	virtual ~OptMethod() = default;
	virtual void optimize(const Area& area, const Function& func, const StopCriterion& crit) const = 0;
	virtual void optimize(const Area& area, const Function& func, const StopCriterion& crit,
		const VectorXd& initPoint) const = 0;
private:
	
};

