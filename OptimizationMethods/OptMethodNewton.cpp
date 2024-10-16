#include "OptMethodNewton.h"

VectorXd getMiddleAreaPoint(const Area& area) {
	
	VectorXd res_point(area.getAreaDim());
	for (size_t i = 0; i < res_point.size(); ++i)
	{
		res_point[i] = (area.getILeftBound(i) + area.getIRightBound(i)) / 2;
	}
	return res_point;
}

void setupData(TransferData& data, const Area& area, const Function& func) {
	double alpha = 1e-2;
	data.setAlpha(alpha);

	size_t curr_iter = 0;
	data.setCurrIter(curr_iter);

	VectorXd curr_point = getMiddleAreaPoint(area);
	data.setCurrPoint(curr_point);

	VectorXd prev_point = curr_point;
	data.setPrevPoint(prev_point);

	double f_val = func(curr_point); // for begin
	double f_prev = f_val;
	data.setCurrFVal(f_val);
	data.setPrevFVal(f_prev);

	VectorXd grad(area.getAreaDim());
	data.setCurrGradient(grad);

}


void OptMethodNewton::doStep(const Area& area, const Function& func,
	const StopCriterion& crit, TransferData& data) const {
	std::shared_ptr<VectorXd> curr_point = data.getCurrPoint();
	std::shared_ptr<VectorXd> prev_point = data.getPrevPoint();
	std::shared_ptr<VectorXd> curr_grad = data.getCurrGradient();
	std::shared_ptr<size_t> curr_iter = data.getCurrIter();
	std::shared_ptr<double> curr_f_val = data.getCurrFVal();
	std::shared_ptr<double> prev_f_val = data.getPrevFVal();
	std::shared_ptr<double> alpha = data.getAlpha();

	
	*curr_grad = func.getGradient(*prev_point);

	//
	MatrixXd mat = func.getGoesseMatrix(*prev_point);
	MatrixXd mat_inv = mat.inverse();
	//
	VectorXd dir = -(func.getGoesseMatrix(*prev_point).inverse() * (*curr_grad));
	*prev_point = *curr_point;
	*curr_point = *prev_point + *data.getAlpha() * dir;
	
	*prev_f_val = *curr_f_val;
	*curr_f_val = func(*curr_point);
	++(*curr_iter);
	data.printData();
}


void OptMethodNewton::optimize(const Area& area, const Function& func, const StopCriterion& crit) const {
	TransferData data;
	setupData(data, area, func);

	doStep(area, func, crit, data); // init step
	
	while (crit.check(data)) {
		doStep(area, func, crit, data);
	}

	/*VectorXd res_x = *data.getCurrPoint();
	for (int i = 0; i < res_x.size(); ++i)
		cout << res_x[i] << " ";
	cout << endl;*/
}