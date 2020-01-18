#include "pch.h"
#include "vOperator.h"

std::vector<double> operator+(std::vector<double> a, std::vector<double> b) {
	for (int i = 0; i < a.size(); i++) {
		a[i] += b[i];
	}
	return a;
}

std::vector<double> operator*(std::vector <double> a, double b) {
	for (int i = 0; i < a.size(); i++) {
		a[i] *= b;
	}
	return a;
}
