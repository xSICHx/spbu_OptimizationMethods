#pragma once
#include "Function.h"


class FucntionTest :
    public Function
{
public:
    ~FucntionTest() = default;

    size_t getDim() const override { return 2; }
    string getName() const override { return "x^2 + y^4"; }

    point_type operator()(const VectorXd& point) const override {
        return point[0] * point[0] + point[1]*point[1]*point[1]*point[1];
    }

};

