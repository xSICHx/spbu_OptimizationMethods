#pragma once
#include "StopCriterionEps.h"
#include "StopCriterionArea.h"


/**
 * @class StopCriterionFEps
 * @brief A class that defines a stopping criterion based on the relative change in function values.
 * 
 * This class inherits from StopCriterionEps and StopCriterionArea and combines their stopping criteria
 * with an additional criterion based on the relative change in function values.
 * 
 */
class StopCriterionFEps :
    public StopCriterionEps,
    public StopCriterionArea
{
public:
    StopCriterionFEps(size_t _n_max, double _eps)
        : StopCriterion(_n_max),
        StopCriterionEps(_n_max, _eps),
        StopCriterionArea(_n_max) {};

    bool check(TransferData& data) const override{
        double f_xn = *data.getCurrFVal();
        double f_x_prev = *data.getPrevFVal();
        bool cond1 = StopCriterion::check(data);
        bool cond2 = abs((f_xn - f_x_prev) / f_xn) >= eps;
        bool cond3 = StopCriterionArea::check(data);
        return cond1 && cond2 && cond3;
    }
    string getName() override {
        return "|f_curr - f_prev|/|f_curr| >= eps";
    }
};

