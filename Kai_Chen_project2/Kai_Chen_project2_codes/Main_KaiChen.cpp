//============================================================================
// Name        : Main_KaiChen.cpp
// Author      : Kai Chen
// RUID        : 183001550
// Date        : 02/12/2018
// Description : Project 2 for Computational Finance
//============================================================================

#include <iostream>
#include <cmath>
#include <fstream>
#include <time.h>
using namespace std;

#include "EuroBarrier.h"

int main()
{
	srand(time(0)); // generate different sets of random numbers
	/* Global Variables */
	double S0 = 100;
	double volatility = 0.3;
	double strike = 110;
	double maturity = 1;
	double divid = 0.02;
	double rfRate = 0.05;
	double barrier = 120;

	EuroBarrier myOption(S0, volatility, strike, maturity, divid, rfRate, barrier); // my option for this project

	cout << "******************* Up-and-Out Barrier Call Option ***********************" << endl;
	clock_t t = clock();
	cout << "Closed-form barrier option price = " << myOption.closeForm() << ", run time = " << (t=clock()-t)*1.0/CLOCKS_PER_SEC << endl;
	cout << "Monte Carlo barrier option price = " << myOption.EulerCall() << ", std error = " << myOption.getSE() << ", run time = " << (t=clock()-t)*1.0/CLOCKS_PER_SEC << endl;
	cout << "Antithetic variates barrier option price = " << myOption.antitheticCall() << ", std error = " << myOption.getSE() << ", run time = " << (t=clock()-t)*1.0/CLOCKS_PER_SEC << endl;
	cout << "Control variates barrier option price = " << myOption.controlCall() << ", std error = " << myOption.getSE() << ", run time = " << (t=clock()-t)*1.0/CLOCKS_PER_SEC << endl;
	//cout << "Control variates vanilla option price = " << myOption.controlVanillaCall() << ", std error = " << myOption.getSE() << ", run time = " << (t=clock()-t)*1.0/CLOCKS_PER_SEC << endl; // This is for answering the second question
	cout << "**************************************************************************" << endl;
	return 0;
}