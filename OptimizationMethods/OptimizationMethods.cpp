#include "config.h"
#include "Function.h"
#include "FucntionTest.h"
#include "Area.h"
#include "StopCriterionFEps.h"
#include "StopCriterionFEpsUnnormalized.h"
#include "OptMethodNewton.h"
#include "OptMethodRandomSearch.h"


void initializeRandomGenerator(unsigned int seed) {
	gen.seed(seed); // Инициализация seed
}

// Function class tests
void testFunctionGreatConditionsNewton() {
	cout << "------------------------------" << endl;
	cout << "Newton test with great conditions" << endl;

	cout << "Function: x^2 + y^4" << endl;
	FucntionTest f1;

	VectorXd point(2);
	point[0] = 2; point[1] = 1;
	cout << "Point: " << point[0] << " " << point[1] << endl;

	point_type f1_point = f1(point);
	cout << "Function in point: " << f1_point << endl;

	VectorXd f1_grad = f1.getGradient(point);
	cout << "Gradient in point: ";
	for (size_t i = 0; i < f1_grad.size(); i++)
	{
		cout << f1_grad[i] << " ";
	}
	cout << endl;

	MatrixXd f1_goesse = f1.getGoesseMatrix(point);
	cout << "Goesse in point: " << endl;
	for (size_t i = 0; i < f1_goesse.rows(); i++)
	{
		for (size_t j = 0; j < f1_goesse.cols(); j++)
		{
			cout << f1_goesse(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl;

	MatrixXd ar(2, 2);
	ar(0, 0) = -3; ar(0, 1) = 6; ar(1, 0) = -5; ar(1, 1) = 8;
	Area area(ar);
	FucntionTest f;

	StopCriterionFEps stop(10000, 1e-5);
	OptMethodNewton opt;
	opt.optimize(area, f, stop);
	cout << "------------------------------" << endl << endl;
}


void testFunctionOptNotInAreaNewton() {
	cout << "------------------------------" << endl;
	cout << "Newton test not in area" << endl;

	cout << "Function: x^2 + y^4" << endl;
	FucntionTest f1;

	VectorXd point(2);
	point[0] = 2; point[1] = 1;
	cout << "Point: " << point[0] << " " << point[1] << endl;

	point_type f1_point = f1(point);
	cout << "Function in point: " << f1_point << endl;

	VectorXd f1_grad = f1.getGradient(point);
	cout << "Gradient in point: ";
	for (size_t i = 0; i < f1_grad.size(); i++)
	{
		cout << f1_grad[i] << " ";
	}
	cout << endl;

	MatrixXd f1_goesse = f1.getGoesseMatrix(point);
	cout << "Goesse in point: " << endl;
	for (size_t i = 0; i < f1_goesse.rows(); i++)
	{
		for (size_t j = 0; j < f1_goesse.cols(); j++)
		{
			cout << f1_goesse(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl;

	MatrixXd ar(2, 2);
	ar(0, 0) = -3; ar(0, 1) = -1; ar(1, 0) = -5; ar(1, 1) = 8;
	Area area(ar);
	FucntionTest f;

	StopCriterionFEps stop(10000, 1e-5);
	OptMethodNewton opt(1e-2);
	opt.optimize(area, f, stop);
	cout << "------------------------------" << endl << endl;
}


void testFunctionRandomSearch() {
	cout << "------------------------------" << endl;
	cout << "Random search test" << endl;

	cout << "Function: x^2 + y^4" << endl;
	FucntionTest f1;

	VectorXd point(2);
	point[0] = 2; point[1] = 1;
	cout << "Point: " << point[0] << " " << point[1] << endl;

	point_type f1_point = f1(point);
	cout << "Function in point: " << f1_point << endl;


	MatrixXd ar(2, 2);
	ar(0, 0) = -3; ar(0, 1) = 2; ar(1, 0) = -5; ar(1, 1) = 8;
	Area area(ar);
	FucntionTest f;

	StopCriterionFEpsUnnormalized stop(10000, 1e-5);
	OptMethodRandomSearch opt(0.5, 0.8);
	opt.optimize(area, f, stop);

	cout << "------------------------------" << endl << endl;
}



int main()
{
	initializeRandomGenerator(100);

	testFunctionGreatConditionsNewton();
	testFunctionOptNotInAreaNewton();
	testFunctionRandomSearch();

	return 0;

}
