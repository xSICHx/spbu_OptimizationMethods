#pragma once
#include "StopCriterion.h"


class StopCriterionArea :
    public virtual StopCriterion //virtual для предотвращения постоянной инициализации n_max 
{
public:
    StopCriterionArea(size_t _n_max) : StopCriterion(_n_max) {};
    bool check(TransferData& data) const override {
        bool flag_in_area = *data.getFlagInBound();
        return flag_in_area;
    }
};

