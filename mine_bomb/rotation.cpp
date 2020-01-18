#include "pch.h"
#include "rotation.h"


rotation::rotation(double pitch, double yaw, double roll) :A(pitch, yaw, roll), RG(pitch, yaw, roll) {
	Angles.resize(3);
	Angles[0] = pitch;
	Angles[1] = yaw;
	Angles[2] = roll;
}

void rotation::fromAnglesToRG() {
	RG.RGPar[0] = cos(0.5*Angles[1])*cos(0.5*Angles[0])*cos(0.5*Angles[2]) - sin(0.5*Angles[1])*sin(0.5*Angles[0])*sin(0.5*Angles[2]);  //ro
	RG.RGPar[1] = sin(0.5*Angles[1])*sin(0.5*Angles[0])*cos(0.5*Angles[2]) + cos(0.5*Angles[1])*cos(0.5*Angles[0])*sin(0.5*Angles[2]);  //l
	RG.RGPar[2] = sin(0.5*Angles[1])*cos(0.5*Angles[0])*cos(0.5*Angles[2]) + cos(0.5*Angles[1])*sin(0.5*Angles[0])*sin(0.5*Angles[2]);  //mu
	RG.RGPar[3] = cos(0.5*Angles[1])*sin(0.5*Angles[0])*cos(0.5*Angles[2]) - sin(0.5*Angles[1])*cos(0.5*Angles[0])*sin(0.5*Angles[2]);  //nu
}

void rotation::fromRGtoAngles() {
	Angles[0] = asin(2 * (RG.RGPar[0] * RG.RGPar[3] + RG.RGPar[1] * RG.RGPar[2]));
	Angles[2] = atan2(2 * (RG.RGPar[0] * RG.RGPar[1] - RG.RGPar[3] * RG.RGPar[2]), (RG.RGPar[0]* RG.RGPar[0] + RG.RGPar[2]* RG.RGPar[2] - RG.RGPar[3]* RG.RGPar[3] - RG.RGPar[1]* RG.RGPar[1]));
	Angles[1] = atan2(2 * (RG.RGPar[0] * RG.RGPar[2] - RG.RGPar[1] * RG.RGPar[3]), (RG.RGPar[0]* RG.RGPar[0] + RG.RGPar[1]* RG.RGPar[1] - RG.RGPar[3]* RG.RGPar[3] - RG.RGPar[2]* RG.RGPar[2]));
}

void rotation::fromRGtoMatrix() {
	A.matr[0][0] = (RG.RGPar[0]* RG.RGPar[0] + RG.RGPar[1]*RG.RGPar[1] - RG.RGPar[2]* RG.RGPar[2] - RG.RGPar[3]* RG.RGPar[3]);
	A.matr[1][0] = 2*(RG.RGPar[0] * RG.RGPar[3] + RG.RGPar[1] * RG.RGPar[2]);
	A.matr[2][0] = 2 * (-RG.RGPar[0] * RG.RGPar[2] + RG.RGPar[1] * RG.RGPar[3]);

	A.matr[0][1] = 2 * (-RG.RGPar[0] * RG.RGPar[3] + RG.RGPar[1] * RG.RGPar[2]);
	A.matr[1][1] = (RG.RGPar[0] * RG.RGPar[0] + RG.RGPar[2] * RG.RGPar[2] - RG.RGPar[3] * RG.RGPar[3] - RG.RGPar[1] * RG.RGPar[1]);
	A.matr[2][1] = 2 * (RG.RGPar[0] * RG.RGPar[1] + RG.RGPar[3] * RG.RGPar[2]);

	A.matr[0][2] = 2 * (RG.RGPar[0] * RG.RGPar[2] + RG.RGPar[1] * RG.RGPar[3]);
	A.matr[1][2] = 2 * (-RG.RGPar[0] * RG.RGPar[1] + RG.RGPar[2] * RG.RGPar[3]);
	A.matr[2][2] = (RG.RGPar[0] * RG.RGPar[0] + RG.RGPar[3] * RG.RGPar[3] - RG.RGPar[1] * RG.RGPar[1] - RG.RGPar[2] * RG.RGPar[2]);
}

void rotation::fromRGtoMatrixT() {
	A.matr[0][0] = (RG.RGPar[0] * RG.RGPar[0] + RG.RGPar[1] * RG.RGPar[1] - RG.RGPar[2] * RG.RGPar[2] - RG.RGPar[3] * RG.RGPar[3]);
	A.matr[0][1] = 2 * (RG.RGPar[0] * RG.RGPar[3] + RG.RGPar[1] * RG.RGPar[2]);
	A.matr[0][2] = 2 * (-RG.RGPar[0] * RG.RGPar[2] + RG.RGPar[1] * RG.RGPar[3]);

	A.matr[1][0] = 2 * (-RG.RGPar[0] * RG.RGPar[3] + RG.RGPar[1] * RG.RGPar[2]);
	A.matr[1][1] = (RG.RGPar[0] * RG.RGPar[0] + RG.RGPar[2] * RG.RGPar[2] - RG.RGPar[3] * RG.RGPar[3] - RG.RGPar[1] * RG.RGPar[1]);
	A.matr[1][2] = 2 * (RG.RGPar[0] * RG.RGPar[1] + RG.RGPar[3] * RG.RGPar[2]);

	A.matr[2][0] = 2 * (RG.RGPar[0] * RG.RGPar[2] + RG.RGPar[1] * RG.RGPar[3]);
	A.matr[2][1] = 2 * (-RG.RGPar[0] * RG.RGPar[1] + RG.RGPar[2] * RG.RGPar[3]);
	A.matr[2][2] = (RG.RGPar[0] * RG.RGPar[0] + RG.RGPar[3] * RG.RGPar[3] - RG.RGPar[1] * RG.RGPar[1] - RG.RGPar[2] * RG.RGPar[2]);
}


rotation::~rotation() {}
