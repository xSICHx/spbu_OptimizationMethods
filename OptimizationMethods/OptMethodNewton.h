#pragma once
#include "OptMethod.h"
class OptMethodNewton :
    public OptMethod
{
public:

    void optimize(const Area& area, const Function& func, const StopCriterion& crit) const override;
private:
    void doStep(const Area& area, const Function& func,
        const StopCriterion& crit, TransferData& data) const override;
};

