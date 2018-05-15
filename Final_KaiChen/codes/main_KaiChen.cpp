//============================================================================
// Name        : main_KaiChen.cpp
// Author      : Kai Chen
// RUID        : 183001550
// Date        : 05/04/2018
// Description : Final Project for Computational Finance
//============================================================================

#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include "PSOR.h"

using namespace std;

double putPayoff(double spot, double strike);


int main()
{
	int num = 30; // size for mesh size
	vector<double> S_grid(num);
	vector<double> t_grid(num);
	vector<double> F(num);
	vector<double> u(num,0.0); // set as u0 first
	vector<vector<double>> A(num,vector<double>(num));
	vector<vector<double>> M(num,vector<double>(num));
	vector<vector<double>> B(num,vector<double>(num));
	vector<double> Y, Z;

	/* some variables that will be used in the program */
	int i=0;
	int j=0;
	double dt = 0.1;
	double T = 1; //maturity
	double K = 100; // strike
	double S_max = 300; // max for stock grid
	double S_min = 0;   // min for stock grid
	double r = 0.02;    // interest rate
	double R = 300; // bound on spatial domain
	double h = (S_max-S_min)/S_grid.size(); // dS

	double sigma1 = 0.3;
	double sigma2 = 0.3;
	double sigma3 = 0.3;
	double rho12 = 0.2;
	double rho13 = 0.2;
	double rho23 = 0.2;
	double n = 3;
	double sigma = sigma1*sigma1+2*rho12*sigma1*sigma2+2*rho13*sigma1*sigma3+sigma2*sigma2+2*rho23*sigma2*sigma3+sigma3*sigma3;

	for(int i=0; i<S_grid.size(); i++)
		S_grid[i] = (S_max-S_min)/S_grid.size() * (i+1);

	/* build matrix A */
	A[0][0] = 0.5*r*h;
	A[0][1] = -(S_grid[0]*S_grid[0] * sigma*sigma) / (2*h) - 0.5*(r*S_grid[0]);

	for(i=1; i<S_grid.size()-1; i++)
	{
		A[i][i] = 0.5*(S_grid[i]*S_grid[i] * sigma*sigma)*(1./h+1./h) + r/2*(h+h);
		A[i][i-1] = -(S_grid[i]*S_grid[i] * sigma*sigma)/(2.*h) + 0.5*r*S_grid[i];
		A[i][i+1] = -(S_grid[i]*S_grid[i] * sigma*sigma)/(2.*h) - 0.5*r*S_grid[i];
	}

	double last = S_grid.size()-1;
	A[last][last] = 0.5*(S_grid[last]*S_grid[last] * sigma*sigma)*(1./h+1./h) + r/2*(h+h);
	A[last][last-1] = -(S_grid[last]*S_grid[last] * sigma*sigma)/(2.*h) + 0.5*r*S_grid[last];

	/* build matrix M */
	double temp = h/6.0;
	M[0][0] = 4*temp;
	M[0][1] = temp;
	for(i=1; i<S_grid.size()-1; i++)
	{
		M[i][i] = 4*temp;
		M[i][i-1] = temp;
		M[i][i+1] = temp;
	}
	M[last][last] = 4*temp;
	M[last][last-1] = temp;

	/* build matrix B */
	for(i=0; i<S_grid.size(); i++)
		for(j=0; j<S_grid.size(); j++)
			B[i][j] = M[i][j] + dt*A[i][j];

	/* build vector f */
	vector<double> f(num,0.2);
	f[0] = putPayoff(exp(-R),K)*(sigma*sigma/(2.*h*h) + (sigma*sigma	/2-r)/(2.*h));
	f.back() = putPayoff(exp(R),K)*(sigma*sigma/(2.*h*h) - (sigma*sigma	/2-r)/(2.*h));

	/* build vector F */
	for(i=0; i< F.size(); i++)
	{
		for(j=0; j<u.size(); j++)
			F[i] = M[i][j]*u[j];
		F[i] += dt*f[i];
	}


	/* Use Projected SOR to solve the LCP */
	PSOR myLCP(A,F,u,0.5); // input lhs(A), rhs(F), initial guess(u), and the parameter for PSOR(w)
	double t = 0;
	while(t<T)
	{
		u = myLCP.solve(A,F,u,0.5);
		//for(i=0; i<u.size(); i++)   // print out the result for each iteration
		//	cout << u[i] * S_grid[i]*100 << endl;
		for(i=0; i< F.size(); i++)     // update F
		{
			for(j=0; j<u.size(); j++)
				F[i] = M[i][j]*u[j];
			F[i] += dt*f[i];
		}
		t += dt;
	}

	return 0;
}

/* payoff of a put option */
double putPayoff(double spot, double strike)
{
	double result = strike-spot > 0? strike-spot:0;
	return result;
}










