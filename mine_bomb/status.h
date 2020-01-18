#pragma once

#include "pch.h"

#include "iostream" 
//#include <vector>
//#include "Constants.h"
#include <fstream>
#include "rotation.h"
#include "Atmosphere.h"
#include "KoefCalc.h"
#include "targetStatus.h"


class status {
public:
	status(std::vector <double> coordinates, double T, int nnee);
	~status();
	double ksiTarget, ksiXZ, ksiXY;
	const double R_EARTH_G = (6371E3);
	const double PI0 = (398600.4418E9);
	void setStageParamAfterSep();
	matrix B;
	Atmosphere GOST4401;

	std::vector <double> ForcePr;
	std::vector <double> ForcePrG;  //проекция 
	std::vector <double> ForceG;  //вектор силы притяжения
	std::vector <double> Torque;  //момент силы
	int flag, flagEng;
	rotation Rot;
	targetStatus Target;

	void nonIntegr();  //пересчет неинтегрируемых параметров
	std::vector <double> rightPart();	//значения производных	
	double roMin, roMax, r, roTarget;
	void printParam( std::ofstream &fout);	//вывод параметров
	void setParam(std::vector <double> b);
	std::vector <double> getParam();

	double yTarget, zTarget, hiEng;
	double a, b;
	status& operator=(const status& right);
protected:
	std::vector<double> parametr;
	/*	parametr [0] = Vx
		parametr [1] = Vy
		parametr [2] = Vz
		parametr [3] = x
		parametr [4] = y
		parametr [5] = z
		parametr [6] = Wx
		parametr [7] = Wy
		parametr [8] = Wz
		parametr [9] = ro
		parametr [10] = l
		parametr [11] = mu
		parametr [12] = nu
		parametr [13] = t

		*/
	//std::vector <std::vector <double>> psiX;
	//std::vector <std::vector <double>> psiV;
	//std::vector<double> u;
	double alpha;
	double betta;
	const double S_M = PI *D_M*D_M/4;
	double M, L;
	const double I_X = I_X0;
	const double I_Y = I_Y0;
	const double I_Z = I_Z0;
	/////TODO: написать модули для определения параметров
	double g;
	/*double Cx = 0.5;
	double Cy = 0.5;
	double Cz = 0.5;*/
	//std::vector <double> ADkoef;
	double density, q, mach, vFullsq;
	double deltaT, deltaN, deltaE;
	std::vector <double> Eng;
	std::vector <double> v;
	std::vector <double> vg;
	//double mu;
	double T_0, tt;
	int n_e;
	int maxNE;

};

