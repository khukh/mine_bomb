#pragma once
class targetStatus
{

public:
	targetStatus(std::vector<double> a);
	~targetStatus();
	double X, Y, Z;
	double Vx, Vy, Vz;
};

