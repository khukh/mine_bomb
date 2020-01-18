#include "pch.h"
#include "RGParam.h"
//#include "matrix.cpp"

RGParam::RGParam(double pitch, double yaw, double roll) {
	RGPar.resize(4);
	//RGAngle.resize(3);
	/*RGAngle[0] = pitch;
	RGAngle[1] = yaw;
	RGAngle[2] = roll;*/

	RGPar[0] = cos(0.5*yaw)*cos(0.5*pitch)*cos(0.5*roll) - sin(0.5*yaw)*sin(0.5*pitch)*sin(0.5*roll);  //ro
	RGPar[1] = sin(0.5*yaw)*sin(0.5*pitch)*cos(0.5*roll) + cos(0.5*yaw)*cos(0.5*pitch)*sin(0.5*roll);  //l
	RGPar[2] = sin(0.5*yaw)*cos(0.5*pitch)*cos(0.5*roll) + cos(0.5*yaw)*sin(0.5*pitch)*sin(0.5*roll);  //mu
	RGPar[3] = cos(0.5*yaw)*sin(0.5*pitch)*cos(0.5*roll) - sin(0.5*yaw)*cos(0.5*pitch)*sin(0.5*roll);  //nu

	
}


void RGParam::norm() {
	double abs = sqrt(pow(RGPar[0], 2) + pow(RGPar[1], 2) + pow(RGPar[2], 2) + pow(RGPar[3], 2));
	for (int i = 0; i < 4; i++) {
		RGPar[i] /= abs;
	}
}

std::vector<double> RGParam::getRGPar() {
	std::vector <double> rgPar = RGPar;
	//?
	return RGPar;
}

void RGParam::setRGPar(double ro, double lymbda, double mu, double nu) {
	RGPar[0] = ro;
	RGPar[1] = lymbda;
	RGPar[2] = mu;
	RGPar[3] = nu;
}

RGParam::~RGParam() {}
