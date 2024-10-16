#include "OptMethodRandomSearch.h"


void OptMethodRandomSearch::setupData(TransferData& data, const Area& area, const Function& func) const {

	size_t curr_iter = 0;
	data.setCurrIter(curr_iter);

	VectorXd curr_point = area.getMiddleAreaPoint();
	data.setCurrPoint(curr_point);

	VectorXd prev_point = area.getRandomAreaPoint();
	data.setPrevPoint(prev_point);

	double f_val = func(curr_point);
	double f_prev = func(prev_point);
	data.setCurrFVal(f_val);
	data.setPrevFVal(f_prev);
	
}

VectorXd OptMethodRandomSearch::getRandomNewPoint(const Area& area, const Area& ball) const {
	std::uniform_real_distribution<> dis(0, 1);
	double choice = dis(gen);
	if (choice > p)
		return area.getRandomAreaPoint();
	
	Area interseption = area.interseption(ball);
	return interseption.getRandomAreaPoint();
	
}

void OptMethodRandomSearch::doStep(
	const Area& area,
	const Function& func,
	const StopCriterion& crit,
	TransferData& data, Area& ball, double& delta) const 
{
	std::shared_ptr<VectorXd> curr_point = data.getCurrPoint();
	std::shared_ptr<VectorXd> prev_point = data.getPrevPoint();
	std::shared_ptr<size_t> curr_iter = data.getCurrIter();
	std::shared_ptr<double> curr_f_val = data.getCurrFVal();
	std::shared_ptr<double> prev_f_val = data.getPrevFVal();

	VectorXd new_potential_point = getRandomNewPoint(area, ball); // y_i
	double f_new_potential = func(new_potential_point);
	if (f_new_potential <= *curr_f_val) {
		*prev_point = *curr_point;
		*prev_f_val = *curr_f_val;

		*curr_point = new_potential_point;
		*curr_f_val = f_new_potential;
		delta *= alpha;
		ball.setCube(*curr_point, delta);
	}
	//cout << delta << endl;

	++(*curr_iter);
}


void OptMethodRandomSearch::optimize(const Area& area, const Function& func, const StopCriterion& crit) const {
	TransferData data;
	setupData(data, area, func);

	// init ball will be twice as low as area
	double delta = area.getMinBoundLen() / 2;
	Area ball(*data.getCurrPoint(), delta); 

	doStep(area, func, crit, data, ball, delta); // init step
	data.printData();

	while (crit.check(data)) {
		doStep(area, func, crit, data, ball, delta);
	}
	data.printData();

}