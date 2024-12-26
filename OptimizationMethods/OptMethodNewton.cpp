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

void OptMethodNewton::setupData(TransferData& data, const Area& area, const Function& func, const VectorXd& initPoint) const {
	data.setAlpha(alpha);

	size_t curr_iter = 0;
	data.setCurrIter(curr_iter);

	VectorXd curr_point = initPoint;
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


double backtrackingAlpha(
	const Function& func,
	std::shared_ptr<VectorXd> prev_point,
	VectorXd& dir,
	std::shared_ptr<double> curr_f_val,
	const Area& area
	) 
{
	double alpha_tmp = 1;
	VectorXd x_prev = *prev_point;
	VectorXd x_new = *prev_point + alpha_tmp * dir;
	double f_prev = *curr_f_val, f_new = func(x_new);
	//cout << dir << endl;
	//while (f_prev < f_new) {
	while (alpha_tmp != 0){
		alpha_tmp /= 2;
		x_new = x_prev + alpha_tmp * dir;
		f_new = func(x_new);
		if (!area.checkPointInArea(x_new))
			continue;
		if (f_prev < f_new) {
			continue;
		}
		else
			break;
	}

	double alpha_tmp2 = -1;
	VectorXd x_prev2 = *prev_point;
	VectorXd x_new2 = *prev_point + alpha_tmp2 * dir;
	double f_prev2 = *curr_f_val, f_new2 = func(x_new2);
	while (alpha_tmp2 != 0) {
		alpha_tmp2 /= 2;
		x_new2 = x_prev2 + alpha_tmp2 * dir;
		f_new2 = func(x_new2);
		if (!area.checkPointInArea(x_new2))
			continue;
		if (f_prev2 < f_new2) {
			continue;
		}
		else
			break;
	}

	return f_new2 > f_new ? alpha_tmp: alpha_tmp2;
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

	
	*curr_grad = func.getGradient(*curr_point);

	
	MatrixXd mat = func.getHessianMatrix(*curr_point);
	VectorXd dir;
	//cout << abs(mat.determinant()) << endl;
	if (-1e-3 <= abs(mat.determinant()) && abs(mat.determinant()) <= 1e-3) {
		dir = -(*curr_grad);
		//cout << dir << endl << endl;
	}
	else
		dir = -(mat.colPivHouseholderQr().solve(*curr_grad));
	//dir = -(*curr_grad);
	//dir = -*curr_grad;

	*prev_point = *curr_point;
	// TMP: backtracking тут, можно было бы переписать отдельной функцией
	*alpha = backtrackingAlpha(func, prev_point, dir, curr_f_val, area)/2;
	//*alpha = 1e-2;
	*curr_point = *prev_point + *alpha * dir;

	if (!area.checkPointInArea(*curr_point)) {
		*flag_in_bounds = false;

		double alpha_curr = 
			max((area.getILeftBound(0) - (*prev_point)[0])/dir[0],
				(area.getIRightBound(0) - (*prev_point)[0]) / dir[0]);
		double alpha_min = alpha_curr;
		for (int i = 1; i < area.getAreaDim(); ++i)
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

void OptMethodNewton::optimize(const Area& area, const Function& func, const StopCriterion& crit, const VectorXd& initPoint) const {
	TransferData data;
	setupData(data, area, func, initPoint);

	data.clearFile();
	doStep(area, func, crit, data); // init step
	data.printData();
	data.savePointToFile();

	while (crit.check(data)) {
		//data.printData();
		doStep(area, func, crit, data);
		data.savePointToFile();
	}
	data.printData();
	
	data.savePointToFile();
	/*VectorXd res_x = *data.getCurrPoint();
	for (int i = 0; i < res_x.size(); ++i)
		cout << res_x[i] << " ";
	cout << endl;*/
}