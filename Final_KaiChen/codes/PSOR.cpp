// class functions for PSOR.h

#include "PSOR.h"
#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
using namespace std;

PSOR::PSOR(vector<vector<double>> A_, vector<double> F_, vector<double> u_, double w_)
			:
			A(A_), F(F_), u(u_), w(w_)
{

}


vector<double> PSOR::solve(vector<vector<double>> A, vector<double> F, vector<double> u, double w)
{
	double sumTemp = 0;
	//double w = 0.5; // parameter for PSOR
	vector<double> u_prev;
	u_prev = u;

	double error = 1;
	
	while(error > epsilon)
	{
		int i=0;
		int j=0;
		error = 0;
		u_prev = u;
		for(i=0; i<u.size(); i++)
		{
			sumTemp = 0;
			for(j=0; j<i-1; j++)
				sumTemp += A[i][j]*u[j];
			for(j=i+1; j<u.size(); j++)
				sumTemp += A[i][j]*u_prev[j];
			u[i] = 1/A[i][i] * (F[i]-sumTemp);
			u[i] = u_prev[i]+ w*(u[i]-u_prev[i]) > 0? u_prev[i]+ w*(u[i]-u_prev[i]):0;
		}

		for(i=0; i<u.size(); i++)
			error += (u[i] - u_prev[i])*(u[i] - u_prev[i]);
	}
	return u;
}

void PSOR::print()
{
	for(int i=0; i<u.size(); i++)
		cout << u[i] << endl;
}








