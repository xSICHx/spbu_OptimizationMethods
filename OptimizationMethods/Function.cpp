#include "Function.h"


VectorXd Function::getGradient(const VectorXd& point) const {
	VectorXd res(getDim());

	VectorXd point_h = point;
	point_type f_in_point = (*this)(point);
	for (size_t i = 0; i < getDim(); ++i)
	{
		point_h[i] += h;
		res[i] = ((*this)(point_h) - f_in_point) / h;
		point_h[i] -= h;
	}

	return res;
}


MatrixXd Function::getHessianMatrix(const VectorXd& point) const {
	MatrixXd res_mat(getDim(), getDim());

	VectorXd point_temp = point;
	for (size_t i = 0; i < getDim(); ++i)
	{
		for (size_t j = 0; j < getDim(); ++j) {
			point_temp[i] += h;
			point_temp[j] += h;
			point_type f1 = (*this)(point_temp);
			point_temp[i] -= h;
			point_temp[j] -= h;

			point_temp[i] += h;
			point_temp[j] -= h;
			point_type f2 = (*this)(point_temp);
			point_temp[i] -= h;
			point_temp[j] += h;

			point_temp[i] -= h;
			point_temp[j] += h;
			point_type f3 = (*this)(point_temp);
			point_temp[i] += h;
			point_temp[j] -= h;

			point_temp[i] -= h;
			point_temp[j] -= h;
			point_type f4 = (*this)(point_temp);
			point_temp[i] += h;
			point_temp[j] += h;

			res_mat(i, j) = (f1 - f2 - f3 + f4)/(4*h*h);
		}
	}
	
	return res_mat;
}