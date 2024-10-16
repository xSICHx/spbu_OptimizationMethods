#pragma once
#include "StopCriterionEps.h"


class StopCriterionFEps :
    public StopCriterionEps
{
public:
    StopCriterionFEps(size_t _n_max, double _eps) : StopCriterionEps(_n_max, _eps) {};
    bool check(TransferData& data) const override{
        double f_xn = *data.getCurrFVal();
        double f_x_prev = *data.getPrevFVal();
        bool cond1 = StopCriterion::check(data);
        bool cond2 = abs((f_xn - f_x_prev) / f_xn) >= eps;
        return cond1 && cond2;
    }
};

