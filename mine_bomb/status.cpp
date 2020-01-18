
#include "pch.h"

#define _USE_MATH_DEFINES
#include "status.h"

#include "cmath"
#include <fstream>



status::status(std::vector <double> coordinates, double T, int nnee): Rot(PITCH0, YAW0, ROLL0), Target(coordinates),B(0,0,0) {
	parametr.resize(14);
	ForcePr.resize(3);
	ForcePrG.resize(3);
	ForceG.resize(3);
	Torque.resize(3);
	v.resize(3);
	vg.resize(3);
	ksiTarget = 0;
	tt = 0;
	M = M0;
	L = L0;
	T_0 = T;
	flagEng = 0;
	yTarget = 0;
	zTarget = 0;
	n_e = nnee;
	maxNE = nnee;
	a = 0;
	b = 0;
	Eng.resize(maxNE);
	for (int i = 0; i < maxNE; i++) {
		Eng[i] = 1;
	}
	//Eng = { 1,1,1,1,1,1,1,1  };//6 двигателей
	//ADkoef.resize(6);
	//скорости
	parametr[0] = V0 * cos(PITCH0) * cos(YAW0);
	parametr[1] = V0 * sin(PITCH0);
	parametr[2] = V0 * cos(PITCH0) * sin(YAW0);
	//кординаты
	parametr[3] = X0;
	parametr[4] = Y0;
	parametr[5] = Z0;
	//угловые скорости
	parametr[6] = 0;
	parametr[7] = 0;
	parametr[8] = 0;
	//параметры Родрига-Гамильтона
	std::vector <double> a = Rot.RG.getRGPar();
	parametr[9] = a[0];
	parametr[10] = a[1];
	parametr[11] = a[2];
	parametr[12] = a[3];

	parametr[13] = 0;
}

status::~status() {}

void status::setStageParamAfterSep( )
{
	M = M1;
	L = L1;
	

}

//значения производных
std::vector <double> status::rightPart() {
	std::vector <double> prir(14);
	//dV/dt
	prir[0] = (ForcePrG[0] + ForceG[0]) / M;
	prir[1] = (ForcePrG[1] + ForceG[1]) / M;
	prir[2] = (ForcePrG[2] + ForceG[2]) / M;
	//dX/dt
	prir[3] = parametr[0];
	prir[4] = parametr[1];
	prir[5] = parametr[2];
	//dW/dt
	prir[6] = Torque[0] / I_X - (I_Z - I_Y) / I_X * parametr[7] * parametr[8];
	prir[7] = Torque[1] / I_Y - (I_X - I_Z) / I_Y * parametr[6] * parametr[8];
	prir[8] = Torque[2] / I_Z - (I_Y - I_X) / I_Z * parametr[6] * parametr[7];
	//dPRG/dt
	prir[9] = -0.5*(parametr[6] * parametr[10] + parametr[7] * parametr[11] + parametr[8] * parametr[12]);
	prir[10] = 0.5*(parametr[6] * parametr[9] - parametr[7] * parametr[12] + parametr[8] * parametr[11]);
	prir[11] = 0.5*(parametr[6] * parametr[12] + parametr[7] * parametr[9] - parametr[8] * parametr[10]);
	prir[12] = 0.5*(-parametr[6] * parametr[11] + parametr[7] * parametr[10] + parametr[8] * parametr[9]);
	//dt/dt
	prir[13] = 1;

	return(prir);

}

void status::nonIntegr() {
	Rot.RG.setRGPar(parametr[9], parametr[10], parametr[11], parametr[12]);
	Rot.RG.norm();
	Rot.fromRGtoAngles();

	
	vg = {parametr[0],parametr[1],parametr[2]};
	Rot.fromRGtoMatrixT();
	v = Rot.A*vg;

	vFullsq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	double vv = sqrt(vFullsq);
	density = GOST4401.roFunc(parametr[4]);
	double ah = atan2(v[1], v[0]);
	alpha = - ah;
	betta = asin(v[2] / sqrt(vFullsq));
	double alphaSpace = sqrt(alpha*alpha + betta * betta);
	mach = vv / GOST4401.aFunc(parametr[4]);

	q = density * vFullsq / 2;

	double Cx = (parametr[13] < T_0) ? cxBase(mach, alphaSpace) : cxKor(mach, alphaSpace);
	Cx = (Cx > 0.8) ? 0.8 : Cx;
	Cx = (Cx < 0.2) ? 0.2 : Cx;
	double Cy = (parametr[13] < T_0) ? cyBase(mach, alpha) : cyKor(mach, alpha);
	double Cz = (parametr[13] < T_0) ? -cyBase(mach, betta) : -cyKor(mach, betta);
	double mz = (parametr[13] < T_0) ? mzBase(mach, alpha) : mzKor(mach, alpha);
	double mzwz = (parametr[13] < T_0) ? mzWzBase(mach, alpha) : mzWzKor(mach, alpha);
	double my = (parametr[13] < T_0) ? mzBase(mach, betta) : mzKor(mach, betta);
	double mywy = (parametr[13] < T_0) ? mzWzBase(mach, betta) : mzWzKor(mach, betta);
	double mx = (parametr[13] < T_0) ? mxBase(mach, alphaSpace) : mxKor(mach, alphaSpace);
	double mxwx = (parametr[13] < T_0) ? mxWxBase(mach, alphaSpace) : mxWxKor(mach, alphaSpace);

	if (flag == 1) {
		std::vector<double> xg = { Target.X - parametr[3], Target.Y - parametr[4], Target.Z - parametr[5] }; //перенос ск в цм ракеты
		Rot.fromRGtoMatrixT();
		std::vector<double> x(3);
		x = Rot.A * xg;
		
		r = sqrt(xg[0] * xg[0] + xg[1] * xg[1] + xg[2] * xg[2]);
		roMin = r * tan(KSI_MIN);
		roMax = r * tan(KSI_MAX);
		roTarget = sqrt(x[1] * x[1] + x[2] * x[2]);
		ksiXY = atan2(x[1], x[0]);
		ksiXZ = atan2(x[2], x[0]);
		ksiTarget = atan2(roTarget, x[0]);

		if (flagEng == 0) {
			
			if (r > 50) {
				if (((roTarget > roMin) && (roTarget < roMax))) {
					double hiTarget = (atan2(x[2], x[1])); // в радианах от -ПИ до ПИ
					hiTarget += (hiTarget < 0) ? 2 * PI : 0;
					double hiEng = hiTarget - PI - parametr[6] * T_ENG / 2;
					hiEng += (hiEng < 0) ? 2 * PI : 0;

					for (n_e = 0; ((n_e * 360 / maxNE - hiEng * toDeg)*(((n_e + 1) % maxNE) * 360 / maxNE - hiEng * toDeg) > 0); n_e++);
					n_e -= (n_e == maxNE) ? 1 : 0;

					if ((abs(n_e * 360 / maxNE + 0.5 * 360 / maxNE - hiEng * toDeg) < (abs(0.1))) && (Eng[n_e] == 1)) {
						flagEng = 1;
						tt = parametr[13];
						Eng[n_e] = 0;
						std::cout << tt << '\t' << ksiXY << '\t';
						std::cout << ksiXZ << '\t' << ksiTarget << '\n';
						//std::cout << hiEng * toDeg << '\n';

						a = -flagEng * cos((360 / maxNE / 2 + 360 / maxNE * n_e)*toRad);
						b = -flagEng * sin((360 / maxNE / 2 + 360 / maxNE * n_e)*toRad);
					}


				}

			}
			
			
			
		} else {
			if ((parametr[13] > tt + T_ENG)) {
				flagEng = 0;
				n_e = maxNE;
				a = 0;
				b = 0;

			}

		}		

	}
	
	ForcePr[0] = -Cx * q * S_M;
	ForcePr[1] = Cy * alpha * q * S_M + a * P * g;
	ForcePr[2] = Cz * betta * q * S_M + b * P * g;
		
	g = 9.80665/*PI0 / ((R_EARTH_G + parametr[4])*(R_EARTH_G + parametr[4]))*/;
	
	Rot.fromRGtoMatrix();
	ForcePrG = Rot.A * ForcePr;
	
	ForceG[0] = 0;
	ForceG[1] = -M * g;
	ForceG[2] = 0;

	//double q = L * density * vFullsq * S_M / 2;
	double kADdevelopment = 0.0;
	double kMVr = 0.1;
	Torque[0] = (mxwx * parametr[6] * D_M / vv + kMVr * mx) * q * S_M * D_M;
	Torque[1] = (mywy * parametr[7] * D_M / vv + my * betta) * q * S_M * L ;
	Torque[2] = (mzwz * parametr[8] * D_M / vv + mz * alpha) * q * S_M * L ;

}

//вывод параметров
void status::printParam(std::ofstream &fout) {
	fout << parametr[13] << '\t' << std::scientific;
	for (int i = 0; i <13; i++) {
		fout << parametr[i] << '\t';
		
	}
	
	fout << alpha * 180 / PI << '\t' << betta * 180 /PI << '\t';

	for (int i = 0; i < 3; i++) {
		fout << ForceG[i] << '\t';
	}
	for (int i = 0; i < 3; i++) {
		fout << ForcePr[i] << '\t';
	}
	for (int i = 0; i < 3; i++) {
		fout << Torque[i] << '\t';
	}
	for (int i = 0; i < 3; i++) {
		fout << Rot.Angles[i]*180/PI << '\t';
	}
	fout << ksiTarget*toDeg << '\t' << a  << '\t' << b << '\t' << ksiXY*toDeg << '\t' << ksiXZ << '\t' << roMin << '\t' << roTarget << '\t' << roMax;
	fout << '\n';

}
void status::setParam(std::vector <double> b) {
	parametr = b;
}

std::vector <double> status::getParam() {
	std::vector <double> get = parametr;
	return(get);
}

status& status::operator=(const status& right) {
	//проверка на самоприсваивание
	if (this == &right) {
		return *this;
	}
	parametr = right.parametr;
	Rot = right.Rot;
	M = M1;
	L = L1;
	a = right.a;
	b = right.b;
	
	flag = right.flag;
	n_e = right.n_e;
	tt = right.tt;
	flagEng = right.flagEng;
	Eng = right.Eng;
	nonIntegr();
	return *this;
}


