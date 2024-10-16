#pragma once
#include "StopCriterion.h"


class StopCriterionEps :
    public virtual StopCriterion //virtual ��� �������������� ���������� ������������� n_max
{
protected:
    double eps;

public:
    StopCriterionEps(size_t _n_max, double _eps) : StopCriterion(_n_max), eps(_eps) {};
};

