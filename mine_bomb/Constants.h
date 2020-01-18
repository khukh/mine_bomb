#pragma once
const double PI = 3.14159265358979323846264338328;
const double toDeg = 180 / PI;
const double toRad = PI / 180;

const int N = 3;

const double X0 = 0;
const double Y0 = 1E-3;
const double Z0 = 0;

const double ALPHA0 = 0;
const double BETTA0 = 0;

const double PITCH0 = 70 * toRad; //тангаж в радианах
const double ROLL0 = (0)*toRad; //угол крена в радианах
const double YAW0 = (0)*toRad; //угол рысканья в радианах

const double W_X0 = 0;
const double W_Y0 = 0;
const double W_Z0 = 0;

const double I_X0 = 0.041;
const double I_Y0 = 0.753;
const double I_Z0 = 0.753;

const double D_M = 0.12;
const double L0 = 0.9;
const double V0 = 444;
//const double V0 = 445 + 0.3 * N; //модуль скорости
const double M0 = 20;

const double H = 0.001;
const double EPS1 = 1E-6;

const double dT = 3.5; // время сброса го до отделения
//парамеры после отделения
const double M1 = 19;
const double L1 = 0.78;

const double P = 500;
const double T_ENG = 0.04;
//int NUM_ENG = 8;
const double KSI_MIN = 2.5 * toRad;
const double KSI_MAX = 13 * toRad;

const double R = 5;
