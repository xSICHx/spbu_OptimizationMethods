#pragma once
#include "Function.h"


class FunctionAbs :
    public Function
{
public:
    ~FunctionAbs() = default;

    size_t getDim() const override { return 2; }
    string getName() const override { return "|x+y|+3*|y-x|"; }

    point_type operator()(const VectorXd& point) const override {
        double x = point[0], y = point[1];
        return abs(x + y) + 3*abs(y - x);
    }

};

