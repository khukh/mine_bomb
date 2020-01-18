#pragma once
#include "pch.h"
class RGParam {
public:
	RGParam(double pitch, double yaw, double roll);
	std::vector <double> RGPar;
	//std::vector <double> RGAngle;

	void norm();
	std::vector <double> getRGPar();
	void setRGPar(double ro, double lymbda, double mu, double nu);
	~RGParam();
};

