#pragma once
#include "Function.h"


class FunctionRosenbrock :
    public Function
{
public:
    ~FunctionRosenbrock() = default;

    size_t getDim() const override { return 2; }
    string getName() const override { return "(1-x)^2 + 100*(y-x^2)^2"; }

    point_type operator()(const VectorXd& point) const override {
        double x = point[0], y = point[1];
        return (1-x)*(1-x) + 100*(y-x*x)*(y-x*x);
    }

};

