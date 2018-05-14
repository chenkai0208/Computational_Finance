// EuroOption class member-function definitions

#include <iostream>
#include <cmath>
#include <time.h>

using namespace std;

#include "EuroOption.h"
#include "Normals.h"   // provides the Hastings approximation to the cumulative normal distribution
#include "Random1.h"   // provides the Box-Muller method to generate samples from the univariate normal distribution

/* constructor */
EuroOption::EuroOption(double S0_, double volatility_, double strike_, 
					   double maturity_, double divid_, double rfRate_)
					   :
					   S0(S0_), volatility(volatility_), strike(strike_),
					   maturity(maturity_), divid(divid_), rfRate(rfRate_)
{
	numOfPaths = 10000; // default number of paths
	timeSteps = 252;    // default number of time steps per year
}

/********************* Call Option ***********************/
/* use Black Scholes formula as the closed-form solution */
double EuroOption::BSCall()
{
	double standardDeviation = volatility * sqrt(maturity);
	double moneyness = log(S0/strike);
	double d1 = (moneyness + (rfRate-divid)*maturity + 0.5*standardDeviation*standardDeviation) / standardDeviation;
	double d2 = d1 - standardDeviation;
	return S0 * exp(-divid*maturity) * CumulativeNormal(d1) - strike * exp(-rfRate*maturity) * CumulativeNormal(d2);

}

/* use single-step exact SDE solution */
double EuroOption::singleStepExactCall()
{
	double result = 0.0;
	double temp = (rfRate - divid - 0.5*volatility*volatility) * maturity;
	double standardDeviation = volatility * sqrt(maturity);
	for (int j = 0; j < numOfPaths; j++)
	{
		double thisGaussian = GetOneGaussianByBoxMuller();
		double ST = S0 * exp(temp + standardDeviation * thisGaussian);
		double thisPayoff = ST - strike;
		thisPayoff = thisPayoff > 0 ? thisPayoff : 0;
		result += thisPayoff;
	}
	double mean = result / numOfPaths;
	mean *= exp(-rfRate*maturity);
	return mean;
}

/* use Euler numerical solution of SDE for spot */
double EuroOption::EulerCall1()
{
	double result = 0.0;
	double mesh = 1.0 / timeSteps;
	double rootMesh = sqrt(mesh);
	double St = S0;
	for (int j = 0; j < numOfPaths; j++)
	{
		St = S0;
		for (int i = 0; i < timeSteps; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			St = St + St*(rfRate-divid)*mesh + St*volatility*rootMesh*thisGaussian;
		}
		double thisPayoff = St - strike;
		thisPayoff = thisPayoff > 0 ? thisPayoff : 0;
		result += thisPayoff;
	}
	double mean = result / numOfPaths;
	mean *= exp(-rfRate*maturity);
	return mean;
}

/* use Euler numerical solution of SDE for log spot */
double EuroOption::EulerCall2()
{
	double result = 0;
	double mesh = 1.0 / timeSteps;
	double rootMesh = sqrt(mesh);
	double St = log(S0);
	double temp = rfRate-divid-0.5*volatility*volatility;
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

/* use Milstein numerical solution of SDE for spot */
double EuroOption::MilsteinCall()
{
	double result = 0.0;
	double mesh = 1.0 / timeSteps;
	double rootMesh = sqrt(mesh);
	double St = S0;
	for (int j = 0; j < numOfPaths; j++)
	{
		St = S0;
		for (int i = 0; i < timeSteps; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			St = St + St*(rfRate-divid)*mesh + St*volatility*rootMesh*thisGaussian 
					+ 0.5*volatility*St*volatility * ((mesh*thisGaussian*thisGaussian) - mesh);
		}
		double thisPayoff = St - strike;
		thisPayoff = thisPayoff > 0 ? thisPayoff : 0;
		result += thisPayoff;
	}
	double mean = 1.0 * result / numOfPaths;
	mean *= exp(-rfRate*maturity);
	return mean;
}


/********************* Put Option ***********************/
/* use Black Scholes formula as the closed-form solution */
double EuroOption::BSPut()
{
	double standardDeviation = volatility * sqrt(maturity);
	double moneyness = log(S0/strike);
	double d1 = (moneyness + (rfRate-divid)*maturity + 0.5*standardDeviation*standardDeviation) / standardDeviation;
	double d2 = d1 - standardDeviation;
	return strike * exp(-rfRate*maturity) * CumulativeNormal(-d2) - S0 * exp(-divid*maturity) * CumulativeNormal(-d1);

}

/* use single-step exact SDE solution */
double EuroOption::singleStepExactPut()
{
	double result = 0.0;
	double temp = (rfRate - divid - 0.5*volatility*volatility) * maturity;
	double standardDeviation = volatility * sqrt(maturity);
	for (int j = 0; j < numOfPaths; j++)
	{
		double thisGaussian = GetOneGaussianByBoxMuller();
		double ST = S0 * exp(temp + standardDeviation * thisGaussian);
		double thisPayoff = strike - ST;
		thisPayoff = thisPayoff > 0 ? thisPayoff : 0;
		result += thisPayoff;
	}
	double mean = result / numOfPaths;
	mean *= exp(-rfRate*maturity);
	return mean;
}

/* use Euler numerical solution of SDE for spot */
double EuroOption::EulerPut1()
{
	double result = 0.0;
	double mesh = 1.0 / timeSteps;
	double rootMesh = sqrt(mesh);
	double St = S0;
	for (int j = 0; j < numOfPaths; j++)
	{
		St = S0;
		for (int i = 0; i < timeSteps; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			St = St + St*(rfRate-divid)*mesh + St*volatility*rootMesh*thisGaussian;
		}
		double thisPayoff = strike - St;
		thisPayoff = thisPayoff > 0 ? thisPayoff : 0;
		result += thisPayoff;
	}
	double mean = result / numOfPaths;
	mean *= exp(-rfRate*maturity);
	return mean;
}

/* use Euler numerical solution of SDE for log spot */
double EuroOption::EulerPut2()
{
	double result = 0;
	double mesh = 1.0 / timeSteps;
	double rootMesh = sqrt(mesh);
	double St = log(S0);
	double temp = rfRate-divid-0.5*volatility*volatility;
	for (int j = 0; j < numOfPaths; j++)
	{
		St = log(S0);
		for (int i = 0; i < timeSteps; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			St = St + temp*mesh + volatility*rootMesh*thisGaussian;
		}
		double thisPayoff = strike - exp(St);
		thisPayoff = thisPayoff > 0 ? thisPayoff : 0;
		result += thisPayoff;
	}
	double mean = result / numOfPaths;
	mean *= exp(-rfRate*maturity);
	return mean;
}

/* use Milstein numerical solution of SDE for spot */
double EuroOption::MilsteinPut()
{
	double result = 0;
	double mesh = 1.0 / timeSteps;
	double rootMesh = sqrt(mesh);
	double St = S0;
	for (int j = 0; j < numOfPaths; j++)
	{
		St = S0;
		for (int i = 0; i < timeSteps; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			St = St + St*(rfRate-divid)*mesh + St*volatility*rootMesh*thisGaussian 
					+ 0.5*volatility*St*volatility * ((mesh*thisGaussian*thisGaussian) - mesh);
		}
		double thisPayoff = strike - St;
		thisPayoff = thisPayoff > 0 ? thisPayoff : 0;
		result += thisPayoff;
	}
	double mean = 1.0 * result / numOfPaths;
	mean *= exp(-rfRate*maturity);
	return mean;
}


/********************* Error Analysis ***********************/
double EuroOption::singleStepExactError()
{
	double a[100];
	double sum = 0;
	double sum2 = 0;
	for (int i=0; i<100; i++)
	{
		a[i] = singleStepExactCall();
		sum += a[i];
	}
	double mean = sum / 100;
	for (int i=0; i<100; i++)
	{
		sum2 += (a[i]-mean) * (a[i]-mean);
	}
	double var = sum / 100;
	return sqrt(var/numOfPaths);
}

double EuroOption::Euler1Error()
{
	double a[100];
	double sum = 0;
	double sum2 = 0;
	for (int i=0; i<100; i++)
	{
		a[i] = EulerCall1();
		sum += a[i];
	}
	double mean = sum / 100;
	for (int i=0; i<100; i++)
	{
		sum2 += (a[i]-mean) * (a[i]-mean);
	}
	double var = sum / 100;
	return sqrt(var/numOfPaths);
}

double EuroOption::Euler2Error()
{
	double a[100];
	double sum = 0;
	double sum2 = 0;
	for (int i=0; i<100; i++)
	{
		a[i] = EulerCall2();
		sum += a[i];
	}
	double mean = sum / 100;
	for (int i=0; i<100; i++)
	{
		sum2 += (a[i]-mean) * (a[i]-mean);
	}
	double var = sum / 100;
	return sqrt(var/numOfPaths);
}

double EuroOption::MilsteinError()
{
	double a[100];
	double sum = 0;
	double sum2 = 0;
	for (int i=0; i<100; i++)
	{
		a[i] = MilsteinCall();
		sum += a[i];
	}
	double mean = sum / 100;
	for (int i=0; i<100; i++)
	{
		sum2 += (a[i]-mean) * (a[i]-mean);
	}
	double var = sum / 100;
	return sqrt(var/numOfPaths);
}






