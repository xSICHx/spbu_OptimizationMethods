#pragma once
#include "OptMethod.h"
#include "Area.h"

class OptMethodRandomSearch :
    public OptMethod
{
private:
    double p;
    double alpha;
public:
    OptMethodRandomSearch(double _p = 0.5, double _alpha = 0.8) : p(_p), alpha(_alpha) {}
    void optimize(const Area& area, const Function& func, const StopCriterion& crit) const override;
private:
    void doStep(const Area& area, const Function& func,
        const StopCriterion& crit, TransferData& data,
        Area& ball, double& delta) const;
    void setupData(TransferData& data, const Area& area, const Function& func) const;

    VectorXd getRandomNewPoint(const Area& area, const Area& ball) const;

};

