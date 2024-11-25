#pragma once
#include "OptMethod.h"


class OptMethodNewton :
    public OptMethod
{
private:
    double alpha;
public:
    OptMethodNewton(double _alpha = 1e-2) : alpha(_alpha) {}

    void optimize(const Area& area, const Function& func, const StopCriterion& crit) const override;
private:
    void doStep(const Area& area, const Function& func,
        const StopCriterion& crit, TransferData& data) const;
    void setupData(TransferData& data, const Area& area, const Function& func) const;
};

