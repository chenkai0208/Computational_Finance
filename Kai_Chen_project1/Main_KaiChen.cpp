//============================================================================
// Name        : Main_KaiChen.cpp
// Author      : Kai Chen
// RUID        : 183001550
// Date        : 01/28/2018
// Description : Project1 for Computational Finance
//============================================================================

/* 
Dear Grader and TA, this time I did not use any IDE to finish the project. 
What I did was just write all the codes and compile them using my gcc compiler.
Sorry for any inconvinience that I might cause. If it is not easy for you to test
my codes, please let me know and I will try my best to test my codes on IDE first 
next time. Thanks for your understanding!
*/

#include <iostream>
#include <cmath>
#include <fstream>
#include <time.h>
using namespace std;

#include "EuroOption.h"

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

	EuroOption myOption(S0, volatility, strike, maturity, divid, rfRate); // my option for this project

	cout << "******************* Call Option ***********************" << endl;
	cout << "Option price using closed-form formula = " << myOption.BSCall() << endl;
	cout << "Option price using single-step exact SDE solution = " << myOption.singleStepExactCall() << endl;
	cout << "Option price using Euler numerical solution of SDE for spot = " << myOption.EulerCall1() << endl;
	cout << "Option price using Euler numerical solution of SDE for log spot = " << myOption.EulerCall2() << endl;
	cout << "Option price using Milstein numerical solution of SDE for spot = " << myOption.MilsteinCall() << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << "******************** Put Option ***********************" << endl;
	cout << "Option price using closed-form formula = " << myOption.BSPut() << endl;
	cout << "Option price using single-step exact SDE solution = " << myOption.singleStepExactPut() << endl;
	cout << "Option price using Euler numerical solution of SDE for spot = " << myOption.EulerPut1() << endl;
	cout << "Option price using Euler numerical solution of SDE for log spot = " << myOption.EulerPut2() << endl;
	cout << "Option price using Milstein numerical solution of SDE for spot = " << myOption.MilsteinPut() << endl;
	cout << "-------------------------------------------------------" << endl;
/*
	// Error functions (very time consuming so I comment them for the convinience of testing codes !!!)
	cout << "****************** Error Analysis *********************" << endl;
	cout << "Single-step method Error = " << myOption.singleStepExactError() << endl;
	cout << "Euler for spot Error = " << myOption.Euler1Error() << endl;
	cout << "Euler for log spot Error = " << myOption.Euler2Error() << endl;
	cout << "Milstein method Error = " << myOption.MilsteinError() << endl;
*/
	return 0;
}