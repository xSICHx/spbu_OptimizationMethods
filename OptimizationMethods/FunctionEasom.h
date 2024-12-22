#pragma once
#include "Function.h"



/// <summary>
/// -cos(x1) * cos(x2) * cos(x3) * cos(x4) * exp(-((x1 - pi)^2 + (x2 - pi)^2 + (x3 - pi)^2 + (x4 - pi)^2) / 16)
/// Global min in x = (PI, PI, PI, PI)
/// </summary>
class FunctionEasom :
    public Function
{
public:
    ~FunctionEasom() = default;

    size_t getDim() const override { return 4; }
    string getName() const override { 
        return "-cos(x1) * cos(x2) * cos(x3) * cos(x4) * exp(-((x1 - pi)^2 + (x2 - pi)^2 + (x3 - pi)^2 + (x4 - pi)^2) / 16)";
    }

    point_type operator()(const VectorXd& x) const override {
        double term1 = -cos(x(0)) * cos(x(1)) * cos(x(2)) * cos(x(3));
        double term2 = exp(-((x(0) - M_PI) * (x(0) - M_PI) +
            (x(1) - M_PI) * (x(1) - M_PI) +
            (x(2) - M_PI) * (x(2) - M_PI) +
            (x(3) - M_PI) * (x(3) - M_PI)) / 16.0);
        return term1 * term2;
    }

};

