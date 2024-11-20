#pragma once
#include "Function.h"


class FunctionDefault :
    public Function
{
public:
    ~FunctionDefault() = default;

    size_t getDim() const override { return 2; }
    string getName() const override { return "5*x^2 - 6*x*y + 5*y^2"; }

    point_type operator()(const VectorXd& point) const override {
        double x = point[0], y = point[1];
        return 5*x*x - 6*x*y + 5*y*y;
    }

};

