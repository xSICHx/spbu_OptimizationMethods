#pragma once
#include "OptMethod.h"

/// <summary>
/// Newton method of optimization
/// </summary>

/**
 * @class OptMethodNewton
 * @brief A class that implements the Newton optimization method.
 * 
 * This class inherits from the OptMethod base class and provides
 * implementations for optimizing a given function within a specified area
 * using the Newton method. It includes methods for performing optimization
 * with and without an initial point, as well as private helper methods for
 * performing optimization steps and setting up transfer data.
 * 
 * @param alpha The step size parameter for the Newton method.
 * 
 * @fn OptMethodNewton::OptMethodNewton(double _alpha)
 * @brief Constructor for the OptMethodNewton class.
 * @param _alpha The initial step size parameter for the Newton method. Default is 1e-2.
 * 
 * @fn void OptMethodNewton::optimize(const Area& area, const Function& func, const StopCriterion& crit) const override
 * @brief Optimizes the given function within the specified area using the Newton method.
 * @param area The area within which to optimize the function.
 * @param func The function to be optimized.
 * @param crit The stopping criterion for the optimization process.
 * 
 * @fn void OptMethodNewton::optimize(const Area& area, const Function& func, const StopCriterion& crit, const VectorXd& initPoint) const override
 * @brief Optimizes the given function within the specified area using the Newton method, starting from an initial point.
 * @param area The area within which to optimize the function.
 * @param func The function to be optimized.
 * @param crit The stopping criterion for the optimization process.
 * @param initPoint The initial point from which to start the optimization.
 * 
 * @fn void OptMethodNewton::doStep(const Area& area, const Function& func, const StopCriterion& crit, TransferData& data) const
 * @brief Performs a single optimization step using the Newton method.
 * @param area The area within which to optimize the function.
 * @param func The function to be optimized.
 * @param crit The stopping criterion for the optimization process.
 * @param data The transfer data used during the optimization process.
 * 
 * @fn void OptMethodNewton::setupData(TransferData& data, const Area& area, const Function& func) const
 * @brief Sets up the transfer data for the optimization process.
 * @param data The transfer data to be set up.
 * @param area The area within which to optimize the function.
 * @param func The function to be optimized.
 * 
 * @fn void OptMethodNewton::setupData(TransferData& data, const Area& area, const Function& func, const VectorXd& initPoint) const
 * @brief Sets up the transfer data for the optimization process, starting from an initial point.
 * @param data The transfer data to be set up.
 * @param area The area within which to optimize the function.
 * @param func The function to be optimized.
 * @param initPoint The initial point from which to start the optimization.
 */
class OptMethodNewton :
    public OptMethod
{
private:
    double alpha;
public:
    OptMethodNewton(double _alpha = 1e-2) : alpha(_alpha) {}

    void optimize(const Area& area, const Function& func, const StopCriterion& crit) const override;
    void optimize(const Area& area, const Function& func, const StopCriterion& crit,
        const VectorXd& initPoint) const override;
private:
    void doStep(const Area& area, const Function& func,
        const StopCriterion& crit, TransferData& data) const;
    void setupData(TransferData& data, const Area& area, const Function& func) const;
    void setupData(TransferData& data, const Area& area, const Function& func, const VectorXd& initPoint) const;
};

