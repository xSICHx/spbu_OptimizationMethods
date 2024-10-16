#pragma once
#include "Area.h"
#include "Function.h"
#include "StopCriterion.h"

class OptMethod
{

public:
	// гей 
	virtual ~OptMethod() = default;
	virtual void optimize(const Area& area, const Function& func, const StopCriterion& crit) const = 0;
private:
	virtual void doStep(const Area& area, const Function& func,
		const StopCriterion& crit, TransferData& data) const = 0;
};

