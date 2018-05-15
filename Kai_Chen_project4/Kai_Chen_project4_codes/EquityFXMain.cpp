//============================================================================
// Name        : Main_KaiChen.cpp
// Author      : Kai Chen
// RUID        : 183001550
// Date        : 03/11/2018
// Description : Project 4 for Computational Finance
//============================================================================
//
//
//		EquityFXMain.cpp
//
//       
//
/*
    uses source files 
    AntiThetic.cpp
    Arrays.cpp,  
    ConvergenceTable.cpp, 
    ExoticBSEngine.cpp 
    ExoticEngine 
    MCStatistics.cpp
    Normals.cpp 
    Parameters.cpp,
    ParkMiller.cpp,
    PathDependent.cpp 
    PathDependentAsian.cpp 
    PayOff3.cpp, 
    PayOffBridge.cpp,
    Random2.cpp,
  */
#include"ParkMiller.h"
#include "SobolGen.h"
#include "Uniforms.h"
#include<iostream>
using namespace std;
#include"MCStatistics.h"
#include"ConvergenceTable.h"
#include"AntiThetic.h"
#include"PathDependentAsian.h"
#include"ExoticBSEngine.h"
#include "Normals.h"
#include <cmath>
#include <cstdlib>
#include <iomanip>

double BSCall(double S0, double strike, double volatility, double maturity, double rfRate);

int main()
{

	double Expiry = 1;
	double Strike = 110; 
	double Spot = 100; 
	double Vol = 0.3; 
	double r = 0.05;
    double d = 0;
	unsigned long NumberOfPaths = 10000;
    unsigned NumberOfDates = 1; // dimension = 1 for vanilla options
    
    /* Closed-form vanilla call option price */
    double close = BSCall(Spot, Strike, Vol, Expiry, r);
    cout << "Closed-form vanilla call option price = " << close << endl;

    PayOffCall thePayOff(Strike);

    MJArray times(NumberOfDates);

    for (unsigned long i=0; i < NumberOfDates; i++)
        times[i] = (i+1.0)*Expiry/NumberOfDates;

    ParametersConstant VolParam(Vol);
    ParametersConstant rParam(r);
    ParametersConstant dParam(d);

    PathDependentAsian theOption(times, Expiry, thePayOff);

    StatisticsMean gatherer;
    ConvergenceTable gathererTwo(gatherer);

    
    /* MC vanilla call proce with Park-Miller uniforms */
    RandomParkMiller generator1(NumberOfDates);
    
    ExoticBSEngine theEngine1(theOption, rParam, dParam, VolParam, generator1, Spot);
    
    theEngine1.DoSimulation(gathererTwo, NumberOfPaths);
    
    vector<vector<double> > results1 =gathererTwo.GetResultsSoFar();
    
    cout << "MC vanilla call price with Park-Miller uniforms = " << results1[results1.size()-1][0] << endl;
    
    
    /* MC vanilla call proce with Park-Miller uniforms and antithetics */
    RandomParkMiller generator(NumberOfDates);
    
    AntiThetic GenTwo(generator);

    ExoticBSEngine theEngine(theOption, rParam, dParam, VolParam, GenTwo, Spot);

    theEngine.DoSimulation(gathererTwo, NumberOfPaths);

    vector<vector<double> > results =gathererTwo.GetResultsSoFar();

    cout << "MC vanilla call price with Park-Miller uniforms and antithetics = " << results[results.size()-1][0] << endl;


    /* MC vanilla call proce with Sobol sequence */
    RandomSobolGen generator2(NumberOfDates);
    
    ExoticBSEngine theEngine2(theOption, rParam, dParam, VolParam, generator2, Spot);
    
    theEngine2.DoSimulation(gathererTwo, NumberOfPaths);
    
    vector<vector<double> > results2 =gathererTwo.GetResultsSoFar();
    
    cout << "QMC vanilla call price with Sobol sequence = " << results2[results1.size()-1][0] << endl;



    /* Uniform distribution pairs generator: You need to change the paths for these functions to print out the generated normal random numbers. I attached my output file in the submitted folder.*/
    //Uniforms test;
    //test.ParkMillerUniforms();
    //test.SobolUniforms();

	return 0;

}


/* use Black Scholes formula as the closed-form solution */
double BSCall(double S0, double strike, double volatility, double maturity, double rfRate)
{
    double standardDeviation = volatility * sqrt(maturity);
    double moneyness = log(S0/strike);
    double d1 = (moneyness + rfRate*maturity + 0.5*standardDeviation*standardDeviation) / standardDeviation;
    double d2 = d1 - standardDeviation;
    return S0 * CumulativeNormal(d1) - strike * exp(-rfRate*maturity) * CumulativeNormal(d2);
}
