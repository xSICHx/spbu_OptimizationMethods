#pragma once
#include "OptMethod.h"
#include "Area.h"

/**
 * @class OptMethodRandomSearch
 * @brief A class that implements the Random Search optimization method.
 * 
 * This class inherits from the OptMethod base class and provides implementations
 * for optimizing a given function within a specified area using the Random Search method.
 * 
 * @param p Probability parameter for the random search.
 * @param alpha Scaling factor for the search area.
 * 
 * @fn OptMethodRandomSearch::OptMethodRandomSearch(double _p, double _alpha)
 * @brief Constructor for the OptMethodRandomSearch class.
 * @param _p Probability parameter for the random search (default is 0.5).
 * @param _alpha Scaling factor for the search area (default is 0.8).
 * 
 * @fn void OptMethodRandomSearch::optimize(const Area& area, const Function& func, const StopCriterion& crit) const override
 * @brief Optimizes the given function within the specified area using the Random Search method.
 * @param area The area within which to perform the optimization.
 * @param func The function to be optimized.
 * @param crit The stopping criterion for the optimization process.
 * 
 * @fn void OptMethodRandomSearch::optimize(const Area& area, const Function& func, const StopCriterion& crit, const VectorXd& initPoint) const override
 * @brief Optimizes the given function within the specified area using the Random Search method, starting from an initial point.
 * @param area The area within which to perform the optimization.
 * @param func The function to be optimized.
 * @param crit The stopping criterion for the optimization process.
 * @param initPoint The initial point from which to start the optimization.
 * 
 * @fn void OptMethodRandomSearch::doStep(const Area& area, const Function& func, const StopCriterion& crit, TransferData& data, Area& ball, double& delta) const
 * @brief Performs a single step of the Random Search optimization.
 * @param area The area within which to perform the optimization.
 * @param func The function to be optimized.
 * @param crit The stopping criterion for the optimization process.
 * @param data Transfer data used during the optimization process.
 * @param ball The search area for the current step.
 * @param delta The step size for the current iteration.
 * 
 * @fn void OptMethodRandomSearch::setupData(TransferData& data, const Area& area, const Function& func) const
 * @brief Sets up the transfer data for the optimization process.
 * @param data Transfer data used during the optimization process.
 * @param area The area within which to perform the optimization.
 * @param func The function to be optimized.
 * 
 * @fn void OptMethodRandomSearch::setupData(TransferData& data, const Area& area, const Function& func, const VectorXd& initPoint) const
 * @brief Sets up the transfer data for the optimization process, starting from an initial point.
 * @param data Transfer data used during the optimization process.
 * @param area The area within which to perform the optimization.
 * @param func The function to be optimized.
 * @param initPoint The initial point from which to start the optimization.
 * 
 * @fn VectorXd OptMethodRandomSearch::getRandomNewPoint(const Area& area, const Area& ball) const
 * @brief Generates a random new point within the specified area and search ball.
 * @param area The area within which to generate the new point.
 * @param ball The search area for generating the new point.
 * @return A randomly generated point within the specified area and search ball.
 */
class OptMethodRandomSearch :
    public OptMethod
{
private:
    double p;
    double alpha;
public:
    OptMethodRandomSearch(double _p = 0.5, double _alpha = 0.8) : p(_p), alpha(_alpha) {}
    void optimize(const Area& area, const Function& func, const StopCriterion& crit) const override;
    void optimize(const Area& area, const Function& func, const StopCriterion& crit,
        const VectorXd& initPoint) const override;
private:
    void doStep(const Area& area, const Function& func,
        const StopCriterion& crit, TransferData& data,
        Area& ball, double& delta) const;
    void setupData(TransferData& data, const Area& area, const Function& func) const;
    void setupData(TransferData& data, const Area& area, const Function& func, const VectorXd& initPoint) const;

    VectorXd getRandomNewPoint(const Area& area, const Area& ball) const;

};

