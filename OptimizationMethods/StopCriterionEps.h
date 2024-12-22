#pragma once
#include "StopCriterion.h"


/**
 * @class StopCriterionEps
 * @brief A class that represents a stopping criterion based on epsilon value.
 * 
 * This class inherits from the StopCriterion class and adds an epsilon value
 * as an additional stopping criterion.
 */
class StopCriterionEps :
    public virtual StopCriterion //virtual для предотвращения постоянной инициализации n_max
{
protected:
    double eps;

public:
    StopCriterionEps(size_t _n_max, double _eps) : StopCriterion(_n_max), eps(_eps) {};
};

