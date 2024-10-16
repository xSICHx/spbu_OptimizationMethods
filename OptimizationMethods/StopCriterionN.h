#pragma once
#include "StopCriterion.h"
class StopCriterionN :
    public StopCriterion
{
public:
    StopCriterionN(size_t _n_max) : StopCriterion(_n_max) {};
    
    bool check(size_t iter) const override {
        if (iter > n_max)
            return false;
        return true;
    };

};

