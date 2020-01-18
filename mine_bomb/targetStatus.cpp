#include "pch.h"
#include "targetStatus.h"


targetStatus::targetStatus(std::vector<double> a)
{
	X = a[0];
	Y = a[1];
	Z = a[2];

	Vx = a[3];
	Vy = a[4];
	Vz = a[5];
}

targetStatus::~targetStatus()
{
}
