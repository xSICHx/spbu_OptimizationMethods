#pragma once
#include "config.h"


class Area
{
private:
	// matrix(n, 2). По первому индексу - верхняя, нижняя границы
	MatrixXd bounds;

public:
	Area(const MatrixXd& _bounds) {
		bounds = _bounds;
	}
	// creates cube with bounds point +- delta
	Area(const VectorXd& point, const double delta) : bounds(point.size(), 2) {
		for (size_t i = 0; i < point.size(); ++i)
		{
			bounds(i, 0) = point[i] - delta;
			bounds(i, 1) = point[i] + delta;
		}
	}

	MatrixXd GetBounds() const {
		return bounds;
	}

	double getILeftBound(const size_t i) const {
		return bounds(i, 0);
	}
	double getIRightBound(const size_t i) const {
		return bounds(i, 1);
	}
	int getAreaDim() const {
		return bounds.rows();
	}
	// true if in area
	bool checkPointInArea(const VectorXd& point) const{
		bool flag = 1;
		for (size_t i = 0; i < getAreaDim(); ++i) {
			flag *= bounds(i, 0) < point[i] && point[i] < bounds(i, 1);
		}
		return flag;
	}

	VectorXd getMiddleAreaPoint() const {
		VectorXd res_point(getAreaDim());
		for (size_t i = 0; i < res_point.size(); ++i)
		{
			res_point[i] = (getILeftBound(i) + getIRightBound(i)) / 2;
		}
		return res_point;
	}

	VectorXd getRandomAreaPoint() const {
		VectorXd res_point(getAreaDim());
		for (size_t i = 0; i < res_point.size(); ++i)
		{
			std::uniform_real_distribution<> dis(getILeftBound(i), getIRightBound(i));
			res_point[i] = dis(gen);
		}
		return res_point;
	}

	double getMinBoundLen() const {
		double res = (getIRightBound(0) - getILeftBound(0));
		for (size_t i = 0; i < getAreaDim(); ++i)
		{
			res = min( (getIRightBound(i) - getILeftBound(i)), res );
		}
		return res;
	}

	// TODO: сделать проверку на пустое
	Area interseption(const Area& another) const {
		MatrixXd inter_bounds(getAreaDim(), 2);
		for (size_t i = 0; i < getAreaDim(); ++i)
		{
			inter_bounds(i, 0) = max(getILeftBound(i), another.getILeftBound(i));
			inter_bounds(i, 1) = min(getIRightBound(i), another.getIRightBound(i));
		}
		Area res(inter_bounds);
		return res;
	}

	// set up cube area with bounds point[i] +- delta
	void setCube(const VectorXd& point, const double delta) {
		for (size_t i = 0; i < point.size(); ++i)
		{
			bounds(i, 0) = point[i] - delta;
			bounds(i, 1) = point[i] + delta;
		}
	}
};

