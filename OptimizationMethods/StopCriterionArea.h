#pragma once
#include "StopCriterion.h"


/**
 * @class StopCriterionArea
 * @brief A class that represents a stopping criterion based on an area condition.
 * 
 * This class inherits from the StopCriterion class and provides an implementation
 * for checking if a stopping criterion based on an area condition is met.
 * 
 * @note The class uses virtual inheritance to prevent repeated initialization of n_max.
 * 
 * @param _n_max The maximum number of iterations allowed.
 */
class StopCriterionArea :
    public virtual StopCriterion //virtual для предотвращения постоянной инициализации n_max 
{
public:
    StopCriterionArea(size_t _n_max) : StopCriterion(_n_max) {};
    bool check(TransferData& data) const override {
        if (data.getFlagInBound() == nullptr)
            return true;
        bool flag_in_area = *data.getFlagInBound();
        return flag_in_area;
    }
};

