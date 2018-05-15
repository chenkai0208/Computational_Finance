//============================================================================
// Name        : Main_KaiChen.cpp
// Author      : Kai Chen
// RUID        : 183001550
// Date        : 02/27/2018
// Description : Project 3 for Computational Finance
//============================================================================
//
//
//		RandomMain3.cpp
//
//       
//
/*
    uses source files 
    AntiThetic.cpp
    Arrays.cpp,  
    ConvergenceTable.cpp, 
    MCStatistics.cpp
    Normals.cpp
    Parameters.cpp,
    ParkMiller.cpp
    PayOff3.cpp, 
    PayOffBridge.cpp,
    Random2.cpp,
    SimpleMC8.cpp 
    Vanilla3.cpp, 
  */
#include"SimpleMC8.h"
#include"ParkMiller.h"
#include"iostream"
using namespace std;
#include"Vanilla3.h"
#include"MCStatistics.h"
#include"ConvergenceTable.h"
#include"AntiThetic.h"
#include"Normals.h"
#include"LEcuyer.h"
#include <cmath>
#include"Random1.h"
#include "StandNorm.h"
#include <time.h>
double BSCall(double S0, double strike, double volatility, double maturity, double rfRate);
double EulerCallBoxMuller(double S0, double strike, double volatility, double maturity, double rfRate, unsigned long numOfPaths, unsigned long timeSteps);
double EulerCallFishman(double S0, double strike, double volatility, double maturity, double rfRate, unsigned long numOfPaths, unsigned long timeSteps);
double EulerCallInverseDist(double S0, double strike, double volatility, double maturity, double rfRate, unsigned long numOfPaths, unsigned long timeSteps);

int main()
{
	double Expiry = 1;
	double Strike = 110; 
	double Spot = 100; 
	double Vol = 0.3; 
	double r = 0.05; 
	unsigned long NumberOfPaths = 100000;
    double exactPrice = BSCall(Spot, Strike, Vol, Expiry, r);

/* Problem 2.1(a) */
/* Closed-form solution from BS formula */
    cout << "Closed-form option price = " << exactPrice << endl;

/* Price using Park-Miller uniform generator */
    PayOffCall thePayOff(Strike);

    VanillaOption theOption(thePayOff, Expiry);

    ParametersConstant VolParam(Vol);
    ParametersConstant rParam(r);

    StatisticsMean gatherer;
    ConvergenceTable gathererTwo(gatherer);

    RandomParkMiller generator(1);
  
    AntiThetic GenTwo(generator);
    
	SimpleMonteCarlo6(theOption,
                                      Spot, 
                                      VolParam,
                                      rParam,
                                      NumberOfPaths,
                                      gathererTwo,
                                      GenTwo);

    vector<vector<double> > results =gathererTwo.GetResultsSoFar();
    
    cout << "MC option price with Park-Miller uniform generator = " << results[results.size()-1][0] << endl;
/* Output for each iteration */

    
    
/* Price using L'Ecuyer uniform generator */
    RandomLEcuyer generator2(1);
    
    AntiThetic GenTwo2(generator2);
    
    SimpleMonteCarlo6(theOption,
                      Spot,
                      VolParam,
                      rParam,
                      NumberOfPaths,
                      gathererTwo,
                      GenTwo2);
    
    vector<vector<double> > results2 =gathererTwo.GetResultsSoFar();
    
    cout << "MC option price with L'Ecuyer uniform generator = " << results2[results.size()-1][0] << endl;

/* Problem 2.1(b) */
    cout << "MC option price with inverse distribution normal generator = " << EulerCallInverseDist(100, 110, 0.3, 1, 0.05, 100000, 252) << endl;
    cout << "MC option price with Box-Muller normal generator = " << EulerCallBoxMuller(100, 110, 0.3, 1, 0.05, 100000, 252) << endl;
    cout << "MC option price with Fishman normal generator = " << EulerCallFishman(100, 110, 0.3, 1, 0.05, 100000, 252) << endl;

/* Problem 2.2 You need to change the paths for these functions to print out the generated normal random numbers. I attached my output file in the submitted folder.
    StandNorm test;
    test.BoxMullerParkMiller();
    test.BoxMullerLEcuyer();
    test.FishmanParkMiller();
    test.FishmanLEcuyer();
    test.InversParkMiller();
    test.InversLEcuyer();
*/
	return 0;
}

/*
 *
 * Copyright (c) 2002
 * Mark Joshi
 *
 * Permission to use, copy, modify, distribute and sell this
 * software for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Mark Joshi makes no representations about the
 * suitability of this software for any purpose. It is provided
 * "as is" without express or implied warranty.
*/

/* use Black Scholes formula as the closed-form solution */
double BSCall(double S0, double strike, double volatility, double maturity, double rfRate)
{
    double standardDeviation = volatility * sqrt(maturity);
    double moneyness = log(S0/strike);
    double d1 = (moneyness + rfRate*maturity + 0.5*standardDeviation*standardDeviation) / standardDeviation;
    double d2 = d1 - standardDeviation;
    return S0 * CumulativeNormal(d1) - strike * exp(-rfRate*maturity) * CumulativeNormal(d2);
}

double EulerCallBoxMuller(double S0, double strike, double volatility, double maturity, double rfRate, unsigned long numOfPaths, unsigned long timeSteps)
{
    double result = 0;
    double mesh = 1.0 / timeSteps;
    double rootMesh = sqrt(mesh);
    double St = log(S0);
    double temp = rfRate-0.5*volatility*volatility;
    for (int j = 0; j < numOfPaths; j++)
    {
        St = log(S0);
        for (int i = 0; i < timeSteps; i++)
        {
            double thisGaussian = GetOneGaussianByBoxMuller();
            St = St + temp*mesh + volatility*rootMesh*thisGaussian;
        }
        double thisPayoff = exp(St) - strike;
        thisPayoff = thisPayoff > 0 ? thisPayoff : 0;
        result += thisPayoff;
    }
    double mean = result / numOfPaths;
    mean *= exp(-rfRate*maturity);
    return mean;
}

double EulerCallFishman(double S0, double strike, double volatility, double maturity, double rfRate, unsigned long numOfPaths, unsigned long timeSteps)
{
    double result = 0;
    double mesh = 1.0 / timeSteps;
    double rootMesh = sqrt(mesh);
    double St = log(S0);
    double temp = rfRate-0.5*volatility*volatility;
    for (int j = 0; j < numOfPaths; j++)
    {
        St = log(S0);
        for (int i = 0; i < timeSteps; i++)
        {
            double thisGaussian = GetOneGaussianByFishman();
            St = St + temp*mesh + volatility*rootMesh*thisGaussian;
        }
        double thisPayoff = exp(St) - strike;
        thisPayoff = thisPayoff > 0 ? thisPayoff : 0;
        result += thisPayoff;
    }
    double mean = result / numOfPaths;
    mean *= exp(-rfRate*maturity);
    return mean;
}

double EulerCallInverseDist(double S0, double strike, double volatility, double maturity, double rfRate, unsigned long numOfPaths, unsigned long timeSteps)
{
    double result = 0;
    double mesh = 1.0 / timeSteps;
    double rootMesh = sqrt(mesh);
    double St = log(S0);
    double temp = rfRate-0.5*volatility*volatility;
    for (int j = 0; j < numOfPaths; j++)
    {
        St = log(S0);
        for (int i = 0; i < timeSteps; i++)
        {
            double thisGaussian = GetOneGaussianByInverseDist();
            St = St + temp*mesh + volatility*rootMesh*thisGaussian;
        }
        double thisPayoff = exp(St) - strike;
        thisPayoff = thisPayoff > 0 ? thisPayoff : 0;
        result += thisPayoff;
    }
    double mean = result / numOfPaths;
    mean *= exp(-rfRate*maturity);
    return mean;
}
