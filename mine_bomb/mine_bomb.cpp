// mine_bomb.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>

#include "Runge.h"
#include "status.h"

int main()
{
	int NUM_ENG = 8;
	std::vector <double> a = {0, 0, 0, 0, 0, 0};
	status bomb1(a, 1000, NUM_ENG);
	bomb1.nonIntegr();
	status buf1(a, 1000, NUM_ENG);
	std::string afilename = "res_nominA.txt";
	std::ofstream fout1(afilename);	
	std::string filename = "res_H2A.txt";
	std::ofstream fout(filename);

	std::string filename_t = "tableNoKorA.txt";
	std::ofstream fout_res_t(filename_t);

	std::string filename_t1 = "tableKorA.txt";
	std::ofstream fout_res_t1(filename_t1);

	double h1 = H;
	while (abs(bomb1.getParam()[4]) > 1E-4) {
		buf1 = bomb1;
		euler(bomb1, h1);
		if (bomb1.getParam()[4] < 0) {
			bomb1 = buf1;
			h1 /= 2;
		}
	}
	std::vector<double> b1 = bomb1.getParam();

	status bomb(a, b1[13] - dT, NUM_ENG);
	bomb.nonIntegr();
	status buf(a, b1[13]-dT, NUM_ENG);
	bomb.printParam(fout1);
	while (bomb.getParam()[13] + H < b1[13] - dT) {
		buf = bomb;
		euler(bomb, H);
		bomb.printParam(fout1);
		if ((fabs(bomb.getParam()[13] - round(bomb.getParam()[13] )) < EPS1)
			&& ((int)round(bomb.getParam()[13]) % 2 == 0)) {
			bomb.printParam(fout_res_t);
		}
	}
	bomb = buf;
	bomb.setStageParamAfterSep();
	bomb.printParam(fout_res_t);
	h1 = H;
	while (abs(bomb.getParam()[4]) > 1E-1) {
		bomb.printParam(fout1);
		if ((fabs(bomb.getParam()[13] - round(bomb.getParam()[13])) < 5E-4)
			&& ((int)round(bomb.getParam()[13]) % 2 == 0)) {
			bomb.printParam(fout_res_t);
		}
		buf = bomb;
		euler(bomb, h1);
		if (bomb.getParam()[4] < 0) {
			bomb = buf;
			h1 /= 2;
		}
	}
	bomb.printParam(fout1);
	bomb.printParam(fout_res_t);
	std::vector<double> b = bomb.getParam();
	status bombKor(a, b[13] - dT, NUM_ENG);
	bombKor.nonIntegr();
	status bufKor(a, b[13] - dT, NUM_ENG);
	while (bombKor.getParam()[13] + H < b[13] - dT) {
		//bufKor = bombKor;
		euler(bombKor, H);
		bombKor.printParam(fout);
		if ((fabs(bombKor.getParam()[13] - round(bombKor.getParam()[13])) < EPS1)
			&& ((int)round(bombKor.getParam()[13]) % 2 == 0)) {
			bombKor.printParam(fout_res_t1);
		}
	}
	euler(bombKor, b[13] - dT - bombKor.getParam()[13]); //отделение ГО
	bombKor.setStageParamAfterSep();
	while (bombKor.getParam()[13] + H < b[13] - (dT - 0.5)) {
		//bufKor = bombKor;
		euler(bombKor, H);
		bombKor.printParam(fout);
		if ((fabs(bombKor.getParam()[13] - round(bombKor.getParam()[13])) < 5E-4)
			&& ((int)round(bombKor.getParam()[13]) % 2 == 0)) {
			bombKor.printParam(fout_res_t1);
		}
	}
	std::cout << bomb.getParam()[13] << '\n';
	bombKor.flag = 1;
	
	double x1 = bomb.getParam()[3]-15;
	double z1 = bomb.getParam()[5]-15;
	std::string filename1 = "res_zoneA.txt";
	std::ofstream fout_res(filename1);

	
	std::vector<double> res;
	/*for (int i = 0; i < 182; i+=2) {
		for (int j = 0; j < 182; j+=2) {*/
			std::vector <double> a1 = { x1 , 0, z1, 0, 0, 0 };
			status bombKor2(a1, bombKor.getParam()[13], NUM_ENG);
			bombKor2 = bombKor;

			status bufKor2(a1, bombKor.getParam()[13], NUM_ENG);
			bufKor2 = bombKor;
			double h2 = H;
			while (abs(bombKor2.getParam()[4]) > 1E-1) {
				bufKor2 = bombKor2;
				euler(bombKor2, h2);
				bombKor2.printParam(fout);
				if ((fabs(bombKor2.getParam()[13] - round(bombKor2.getParam()[13])) < 5E-4)
					&& ((int)round(bombKor2.getParam()[13]) % 2 == 0)) {
					bombKor2.printParam(fout_res_t1);
				}
				if (bombKor2.getParam()[4] < 0) {
					bombKor2 = bufKor2;
					h2 /= 2;
				}
			}
			bombKor2.printParam(fout_res_t1);
			bombKor2.printParam(fout);
			double x = x1 - bombKor2.getParam()[3];
			double z = z1 - bombKor2.getParam()[5];
			double r = sqrt(x*x + z * z);
			std::cout << r << '\n';
			/*if (r < 5) {
				fout_res << x1 + i << '\t' << z1 + j << '\n';
			}
		}
		std::cout << '\n' << i << '\n';
	}*/
	
		
					//traj1[i].printParam(fout2);
				//}
	//bombKor.printParam(fout);
	/*double x = x1 - bombKor2.getParam()[3];
	double z = z1 - bombKor2.getParam()[5];
	double r = sqrt(x*x + z * z);*/
}



