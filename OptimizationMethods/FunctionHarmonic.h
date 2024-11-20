#pragma once
#include "Function.h"



class FunctionHarmonic :
    public Function
{
public:
    ~FunctionHarmonic() = default;

    size_t getDim() const override { return 3; }
    string getName() const override { return "sin(x)*cos(2y)*sin(3z+pi/6)"; }

    point_type operator()(const VectorXd& point) const override {
        double x = point[0], y = point[1], z = point[2];
        double pi = atan(1) * 4;
        return sin(x)*cos(2*y)*sin(3*z+pi/6);
    }

};

