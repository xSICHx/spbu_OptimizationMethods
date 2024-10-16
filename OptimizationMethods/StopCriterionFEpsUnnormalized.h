#pragma once
#include "StopCriterionEps.h"
#include "StopCriterionArea.h"


class StopCriterionFEpsUnnormalized :
    public StopCriterionEps,
    public StopCriterionArea
{
public:
    StopCriterionFEpsUnnormalized(size_t _n_max, double _eps)
        : StopCriterion(_n_max),
        StopCriterionEps(_n_max, _eps),
        StopCriterionArea(_n_max) {};

    bool check(TransferData& data) const override {
        double f_xn = *data.getCurrFVal();
        double f_x_prev = *data.getPrevFVal();
        bool cond1 = StopCriterion::check(data);
        bool cond2 = abs((f_xn - f_x_prev)) >= eps;
        bool cond3 = StopCriterionArea::check(data);
        return cond1 && cond2 && cond3;
    }
};
