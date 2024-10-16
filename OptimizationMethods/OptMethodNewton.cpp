#include "OptMethodNewton.h"


void OptMethodNewton::setupData(TransferData& data, const Area& area, const Function& func) const {
	data.setAlpha(alpha);

	size_t curr_iter = 0;
	data.setCurrIter(curr_iter);

	VectorXd curr_point = area.getMiddleAreaPoint();
	data.setCurrPoint(curr_point);

	VectorXd prev_point = curr_point;
	data.setPrevPoint(prev_point);

	double f_val = func(curr_point); // for begin
	double f_prev = f_val;
	data.setCurrFVal(f_val);
	data.setPrevFVal(f_prev);

	VectorXd grad(area.getAreaDim());
	data.setCurrGradient(grad);

	bool flag_in_bounds = true;
	data.setFlagInBound(flag_in_bounds);

}


void OptMethodNewton::doStep(
	const Area& area,
	const Function& func,
	const StopCriterion& crit,
	TransferData& data) const {

	std::shared_ptr<VectorXd> curr_point = data.getCurrPoint();
	std::shared_ptr<VectorXd> prev_point = data.getPrevPoint();
	std::shared_ptr<VectorXd> curr_grad = data.getCurrGradient();
	std::shared_ptr<size_t> curr_iter = data.getCurrIter();
	std::shared_ptr<double> curr_f_val = data.getCurrFVal();
	std::shared_ptr<double> prev_f_val = data.getPrevFVal();
	std::shared_ptr<double> alpha = data.getAlpha();
	shared_ptr<bool> flag_in_bounds = data.getFlagInBound();

	
	*curr_grad = func.getGradient(*prev_point);

	//
	MatrixXd mat = func.getGoesseMatrix(*prev_point);
	MatrixXd mat_inv = mat.inverse();
	//
	VectorXd dir = -(func.getGoesseMatrix(*prev_point).inverse() * (*curr_grad));
	*prev_point = *curr_point;
	*curr_point = *prev_point + *data.getAlpha() * dir;
	if (!area.checkPointInArea(*curr_point)) {
		*flag_in_bounds = false;

		double alpha_curr = 
			max((area.getILeftBound(0) - (*prev_point)[0])/dir[0],
				(area.getIRightBound(0) - (*prev_point)[0]) / dir[0]);
		double alpha_min = alpha_curr;
		for (size_t i = 1; i < area.getAreaDim(); ++i)
		{
			alpha_curr = 
				max((area.getILeftBound(i) - (*prev_point)[i]) / dir[i],
					(area.getIRightBound(i) - (*prev_point)[i]) / dir[i]);
			alpha_min = min(alpha_min, alpha_curr);
		}
		*curr_point = *prev_point + alpha_min * dir;

	}
	
	*prev_f_val = *curr_f_val;
	*curr_f_val = func(*curr_point);
	++(*curr_iter);
}


void OptMethodNewton::optimize(const Area& area, const Function& func, const StopCriterion& crit) const {
	TransferData data;
	setupData(data, area, func);

	doStep(area, func, crit, data); // init step
	data.printData();
	
	while (crit.check(data)) {
		doStep(area, func, crit, data);
	}
	data.printData();

	/*VectorXd res_x = *data.getCurrPoint();
	for (int i = 0; i < res_x.size(); ++i)
		cout << res_x[i] << " ";
	cout << endl;*/
}