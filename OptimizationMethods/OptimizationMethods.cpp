#include "config.h"
#include "Function.h"
#include "FucntionTest.h"
#include "FunctionAbs.h"
#include "FunctionDefault.h"
#include "FunctionHarmonic.h"
#include "FunctionRosenbrock.h"
#include "FunctionEasom.h"
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
	FunctionTest f1;

	VectorXd point(2);
	point[0] = 2; point[1] = 1;
	cout << "Point: " << point[0] << " " << point[1] << endl;

	point_type f1_point = f1(point);
	cout << "Function in point: " << f1_point << endl;

	VectorXd f1_grad = f1.getGradient(point);
	cout << "Gradient in point: ";
	for (int i = 0; i < f1_grad.size(); i++)
	{
		cout << f1_grad[i] << " ";
	}
	cout << endl;

	MatrixXd f1_hessian = f1.getHessianMatrix(point);
	cout << "Hessian in point: " << endl;
	for (int i = 0; i < f1_hessian.rows(); i++)
	{
		for (int j = 0; j < f1_hessian.cols(); j++)
		{
			cout << f1_hessian(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl;

	MatrixXd ar(2, 2);
	ar(0, 0) = -3; ar(0, 1) = 6; ar(1, 0) = -5; ar(1, 1) = 8;
	Area area(ar);
	FunctionTest f;

	StopCriterionFEps stop(10000, 1e-5);
	OptMethodNewton opt;
	opt.optimize(area, f, stop);
	cout << "------------------------------" << endl << endl;
}


void testFunctionOptNotInAreaNewton() {
	cout << "------------------------------" << endl;
	cout << "Newton test not in area" << endl;

	cout << "Function: x^2 + y^4" << endl;
	FunctionTest f1;

	VectorXd point(2);
	point[0] = 2; point[1] = 1;
	cout << "Point: " << point[0] << " " << point[1] << endl;

	point_type f1_point = f1(point);
	cout << "Function in point: " << f1_point << endl;

	VectorXd f1_grad = f1.getGradient(point);
	cout << "Gradient in point: ";
	for (int i = 0; i < f1_grad.size(); i++)
	{
		cout << f1_grad[i] << " ";
	}
	cout << endl;

	MatrixXd f1_hessian = f1.getHessianMatrix(point);
	cout << "Hessian in point: " << endl;
	for (int i = 0; i < f1_hessian.rows(); i++)
	{
		for (int j = 0; j < f1_hessian.cols(); j++)
		{
			cout << f1_hessian(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl;

	MatrixXd ar(2, 2);
	ar(0, 0) = -3; ar(0, 1) = -1; ar(1, 0) = -5; ar(1, 1) = 8;
	Area area(ar);
	FunctionTest f;

	StopCriterionFEps stop(10000, 1e-5);
	OptMethodNewton opt(1e-2);
	opt.optimize(area, f, stop);
	cout << "------------------------------" << endl << endl;
}


void testFunctionRandomSearch() {
	cout << "------------------------------" << endl;
	cout << "Random search test" << endl;

	cout << "Function: x^2 + y^4" << endl;
	FunctionTest f1;

	VectorXd point(2);
	point[0] = 2; point[1] = 1;
	cout << "Point: " << point[0] << " " << point[1] << endl;

	point_type f1_point = f1(point);
	cout << "Function in point: " << f1_point << endl;


	MatrixXd ar(2, 2);
	ar(0, 0) = -3; ar(0, 1) = 2; ar(1, 0) = -5; ar(1, 1) = 8;
	Area area(ar);
	FunctionTest f;

	StopCriterionFEpsUnnormalized stop(10000, 1e-5);
	OptMethodRandomSearch opt(0.5, 0.8);
	opt.optimize(area, f, stop);

	cout << "------------------------------" << endl << endl;
}


// Функция для запроса числа в диапазоне [min, max]
int getValidInput(int min, int max, const string& prompt) {
	int input;
	while (true) {
		cout << prompt;
		cin >> input;
		if (cin.fail() || input < min || input > max) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a number between " << min << " and " << max << ".\n";
		}
		else {
			break;
		}
	}
	return input;
}

double getValidDouble(const string& prompt) {
	double value;
	while (true) {
		cout << prompt;
		cin >> value;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a valid number.\n";
		}
		else {
			break;
		}
	}
	return value;
}

double getValidDouble(const string& prompt, double min, double max) {
	double value;
	while (true) {
		cout << prompt;
		cin >> value;
		if (cin.fail() || value < min || value > max) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a valid number.\n";
		}
		else {
			break;
		}
	}
	return value;
}


// Функция для заполнения строки матрицы
void getValidRow(VectorXd& row, int rowIndex) {
	while (true) {
		cout << "Enter two numbers for row " << rowIndex + 1 << " (first < second):\n";
		double first = getValidDouble("  First: ");
		double second = getValidDouble("  Second: ");
		if (first < second) {
			row[0] = first;
			row[1] = second;
			break;
		}
		else {
			cout << "Invalid input. The first number must be strictly less than the second.\n";
		}
	}
}


void getValidInitPoint(VectorXd& initPoint, const Area& area) {
	for (int i = 0; i < area.getAreaDim(); i++) {
		initPoint[i] = getValidDouble("Enter point for " + to_string(i) + " dimension :",
			area.getILeftBound(i), area.getIRightBound(i));
	}
}


/**
 * @brief Handles console input for optimization methods.
 * 
 * This function interacts with the user through the console to gather necessary inputs
 * for performing optimization. It allows the user to select the dimension of the space,
 * choose a function to optimize, define the area of optimization, specify the initial point,
 * select a stopping criterion, and choose an optimization method. Once all inputs are gathered,
 * it performs the optimization and displays the results.
 * 
 * The function supports dimensions from 1 to 4 and provides a set of predefined functions,
 * stopping criteria, and optimization methods for the user to choose from.
 * 
 * The following steps are performed:
 * 1. Request the dimension of the space.
 * 2. Display available functions for the selected dimension and request function selection.
 * 3. Request and display the n-dimensional cube (area of optimization).
 * 4. Request the initial point within the defined area.
 * 5. Display available stopping criteria and request criterion selection.
 * 6. Display available optimization methods and request method selection.
 * 7. Perform the optimization using the selected inputs and display the results.
 */
void consoleInput() {
	// Храним функции для каждой размерности
	vector<vector<shared_ptr<Function>>> functions(5);


	// Добавляем функции для размерности 2
	functions[2].emplace_back(make_shared<FunctionDefault>());
	functions[2].emplace_back(make_shared<FunctionAbs>());
	functions[2].emplace_back(make_shared<FunctionRosenbrock>());

	functions[3].emplace_back(make_shared<FunctionHarmonic>());

	functions[4].emplace_back(make_shared<FunctionEasom>());


	int dimension;
	shared_ptr<Function> func;
	shared_ptr<Area> area;
	shared_ptr<StopCriterion> crit;
	shared_ptr<OptMethod> opt;
	VectorXd startPoint;
	while (true) {
		// Request the dimension
		dimension = getValidInput(1, 4, "Enter the dimension of the space (from 1 to 4): ");
		const auto& availableFunctions = functions[dimension];

		if (availableFunctions.empty()) {
			cout << "There are no functions available for dimension " << dimension << ". Please choose a different dimension.\n";
			continue;
		}

		// Display available functions
		cout << "Available functions for dimension " << dimension << ":\n";
		for (size_t i = 0; i < availableFunctions.size(); ++i) {
			cout << i << ": " << availableFunctions[i]->getName() << endl;
		}

		// Request function selection
		int functionIndex = getValidInput(0, availableFunctions.size() - 1,
			"Enter the function number: ");
		cout << "You selected the function: " << availableFunctions[functionIndex]->getName() << endl;
		func = availableFunctions[functionIndex];

		// Создаем матрицу dimension x 2
		MatrixXd ar(dimension, 2);

		// Заполняем матрицу
		for (int i = 0; i < dimension; ++i) {
			VectorXd row(2); // Временный вектор для строки
			getValidRow(row, i);
			ar.row(i) = row.transpose(); // Копируем строку в матрицу
		}

		// Выводим результат
		cout << "You have entered the following n-dimensional cube:\n" << ar << endl;
		area = make_shared<Area>(ar);

		// Ввод начальной точки 
		cout << "Enter initial point: \n";
		VectorXd initPoint = VectorXd::Zero(dimension);
		getValidInitPoint(initPoint, *area);
		startPoint = initPoint;


		// Display available crits
		int max_iter = getValidInput(10, 100000, "Enter the maximum of iterations: ");
		double eps = getValidDouble("Enter eps:");
		
		vector<shared_ptr<StopCriterion>> crits;
		crits.emplace_back(make_shared<StopCriterion>(max_iter));
		crits.emplace_back(make_shared<StopCriterionFEps>(max_iter, eps));
		crits.emplace_back(make_shared<StopCriterionFEpsUnnormalized>(max_iter, eps));

		cout << "Available criterions " << ":\n";
		for (size_t i = 0; i < crits.size(); ++i) {
			cout << i << ": " << crits[i]->getName() << endl;
		}
		int critIndex = getValidInput(0, crits.size() - 1,
			"Enter the criterion number: ");
		cout << "You selected the criterion: " << crits[critIndex]->getName() << endl;
		crit = crits[critIndex];

		// OptMethod
		vector<shared_ptr<OptMethod>> opts;
		opts.emplace_back(make_shared<OptMethodNewton>());
		opts.emplace_back(make_shared<OptMethodRandomSearch>());

		cout << "Available optimize methods" << ":\n";
		cout << 0 << ": " << "Newton" << endl;
		cout << 1 << ": " << "Random Search" << endl;
		int optIndex = getValidInput(0, opts.size() - 1,
			"Enter the optimize method number: ");
		opt = opts[optIndex];
		

		break;
	}
	cout << "------------------------------" << endl << endl;

	opt->optimize(*area, *func, *crit, startPoint);

	cout << "------------------------------" << endl << endl;


}


/*** 
*** ПРИМЕРЫ ***
Newton Rosenbrock (1-x)^2 + 100*(y-x^2)^2 минимум в области
2
2
-5
3
-3
5
-1.35
2
10000
0.0001
2
0

Newton Rosenbrock (1-x)^2 + 100*(y-x^2)^2 минимум вне области
2
2
-5
3
-3
-1
-2
-1.25
10000
0.0001
2
0

Newton 5*x^2 - 6*x*y + 5*y^2 минимум в области
2
0
-10
15
-10
15
-7
9
10000
0.0001
2
0


Newton 5*x^2 - 6*x*y + 5*y^2 минимум вне области
2
0
-10
15
-10
-1
-7
-9
10000
0.00000001
1
0


 
Random Search Rosenbrock (1-x)^2 + 100*(y-x^2)^2 минимум в области
2
2
-5
3
-3
5
-1.35
2
10000
0.001
2
1

Random Search Rosenbrock (1-x)^2 + 100*(y-x^2)^2 минимум вне области
2
2
-5
3
-3
-1
-2
-1.25
10000
0.0001
2
1


Newton |x+y|+3*|y-x| минимум в области
2
1
-15
5
-15
6
3.7
-1
100000
0.00001
2
0


Newton |x+y|+3*|y-x| минимум вне области
2
1
-15
5
1
6
3.7
1
100000
0.00001
2
0


Random Search |x+y|+3*|y-x| минимум в области
2
1
-15
5
-15
6
3.7
-1
100000
0.00001
2
1


Newton sin(x)*cos(2y)*sin(3z+pi/6) минимум в области
3
0
0
3
-0.7
0.7
-0.9
-0.175
0.1
0.3
-0.4
10000
0.00001
2
0


Newton 
-cos(x1) * cos(x2) * cos(x3) * cos(x4) * exp(-((x1 - pi)^2 + (x2 - pi)^2 + (x3 - pi)^2 + (x4 - pi)^2) / 16)
минимум в области
4
0
-3
5
-2
10
-7
8
-10
10
-1
0
4
2
10000
0.000001
1
0


Random Search
-cos(x1) * cos(x2) * cos(x3) * cos(x4) * exp(-((x1 - pi)^2 + (x2 - pi)^2 + (x3 - pi)^2 + (x4 - pi)^2) / 16)
минимум в области
4
0
-3
5
-2
10
-7
8
-10
10
-1
0
4
2
10000
0.000001
1
1

***/


int main()
{

	initializeRandomGenerator(100);

	consoleInput();

	/*testFunctionGreatConditionsNewton();
	testFunctionOptNotInAreaNewton();
	testFunctionRandomSearch();*/


	return 0;

}
