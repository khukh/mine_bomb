#pragma once
class Atmosphere {
public:

	const double R = 287.05287;
	const double GST = 9.80665;
	const double PST = 101325.0;
	const double TST = 288.15;
	const double BS = 1.458E-6;
	const double S = 110.4;
	

	double gPotHStand[9] = {-2.0E+3, 0.0, 11.0E+3, 20.0E+3, 32.0E+3, 47.0E+3, 51.0E+3, 71.0E+3, 85.0E+3};

	double pGostStand[9];
	double temperature[9];

	Atmosphere();

	double pFunc(double h);
	double tFunc(double h);
	double roFunc(double h);
	double aFunc(double h);
	double muFunc(double h);
	double nuFunc(double h);
	
	~Atmosphere();
	//double tfunc(double h);
private:
	void fillArrPandT();
	
	
	double hFromgH(double gH);
	double gHFh(double h);
};

