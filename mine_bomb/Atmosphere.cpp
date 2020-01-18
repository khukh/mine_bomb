#include "pch.h"
#include "Atmosphere.h"

//конструктор
Atmosphere::Atmosphere() {
	fillArrPandT();
}



// функция по заданной геопотенциальной высоте возвращает бета = dT/dH
double betaKoef(double geoPotH) {
	//значения геопотенциальной высоты - первая строчка массива в м
	//значения коэффициента бета=dT/dH - вторая строка массива
	double Beta[2][9] = {
		{ -2000.0, 0.0, 11000.0, 20000.0, 32000.0, 47000.0, 51000.0, 71000.0, 85000.0 },
		{ -6.5E-3, -6.5E-3, 0.0, 1.0E-3, 2.8E-3, 0.0, -2.8E-3, -2.0E-3, 0.0 }
	};
	int i = 0;
	while ((geoPotH >= Beta[0][i + 1])&(i < 8)) i++;

	return (Beta[1][i]);
}
int index(double geoPotH) {
	//значения геопотенциальной высоты - первая строчка массива в м
	//значения коэффициента бета=dT/dH - вторая строка массива
	double gH[9] = {-2000.0, 0.0, 11000.0, 20000.0, 32000.0, 47000.0, 51000.0, 71000.0, 85000.0};
	int i = 0;
	while ((geoPotH >= gH[i + 1])&(i < 8)) i++;

	return (i);
}
double Atmosphere::tFunc(double h) {  // h = геометрическая высота
	double gh = gHFh(h);  //геопотенциальная высота
	double b = betaKoef(gh);
	int i = index(gh);
	double t = temperature[i] + b * (gh - gPotHStand[i]);
	return t;
}


//плотность
double Atmosphere::roFunc(double h) {
	double t = tFunc(h);
	double p = pFunc(h);
	double ro = p / (R*t);
	return (ro);
}
//скорость звука
double Atmosphere::aFunc(double h) {
	double t = tFunc(h);
	double a = 20.046796*sqrt(t);
	return (a);
}
// динамическая вязкость воздуха
double Atmosphere::muFunc(double h) {
	double t = tFunc(h);
	double mu = BS * pow(t, 1.5) / (t + S);
	return (mu);
}
// кинематическая вязкость
double Atmosphere::nuFunc(double h) {
	double mu = muFunc(h);
	double ro = roFunc(h);
	double nu = mu / ro;
	return (nu);
}
// геометрическая высота
double Atmosphere::hFromgH(double gH) {
	const double RZU = 6356767.0;
	double h = RZU * gH / (RZU - gH);
	return (h);
}
//геопотенциальная высота
double Atmosphere::gHFh(double h) {
	const double RZU = 6356767.0;
	double gH = RZU * h / (RZU + h);
	return (gH);
}

void Atmosphere::fillArrPandT() {
	double b;
	for (int i = 0; i < 9; i++) {

		switch (i) {
			case 0:

				b = betaKoef(gPotHStand[0]);
				temperature[0] = TST + b * (gPotHStand[0] - gPotHStand[1]);
				if (b == 0)
					pGostStand[0] = PST * exp(GST*(gPotHStand[1] - gPotHStand[0]) / (R * TST));
				else pGostStand[0] = PST * pow((1.0 + b * ((gPotHStand[1] - gPotHStand[0]) / temperature[0])), (GST / (b *R)));
				break;
			case 1:
				pGostStand[1] = PST;
				temperature[1] = TST;
				break;
			default:
				b = betaKoef(gPotHStand[i - 1]);
				temperature[i] = temperature[i - 1] + b * (gPotHStand[i] - gPotHStand[i - 1]);

				if (b == 0)
					pGostStand[i] = pGostStand[i - 1] * exp((-1)*GST*(gPotHStand[i] - gPotHStand[i - 1]) / (R*temperature[i]));
				//pGostStand[i] = pGostStand[i - 1] * pow(2.71829, ((-1)*GST*(gPotHStand[i] - gPotHStand[i - 1]) / (R*temperature[i])));
				else pGostStand[i] = pGostStand[i - 1] * pow((1 + b * ((gPotHStand[i] - gPotHStand[i - 1]) / temperature[i - 1])), ((-1)*GST / (b * R)));
		}
		//pGostStand[i] = roundBan1(pGostStand[i]);
	}
	return;
}
double Atmosphere::pFunc(double h) {
	double gh = gHFh(h);  //геопотенциальная высота
	double b = betaKoef(gh);
	int i = index(gh);
	double tPr = temperature[i] + b * (gh - gPotHStand[i]);
	double pPr;
	if (b == 0)
		pPr = pGostStand[i] * exp((-1)*GST*(gh - gPotHStand[i]) / (R*tPr));
	else pPr = pGostStand[i] * pow((1 + b * ((gh - gPotHStand[i]) / temperature[i])), ((-1)*GST / (b * R)));
	return (pPr);
}

Atmosphere::~Atmosphere() {}



