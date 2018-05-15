// EuroBarrier class member-function definitions

#include <iostream>
#include <cmath>
#include <time.h>

using namespace std;

#include "EuroBarrier.h"
#include "Normals.h"   // provides the Hastings approximation to the cumulative normal distribution
#include "Random1.h"   // provides the Box-Muller method to generate samples from the univariate normal distribution

/* constructor */
EuroBarrier::EuroBarrier(double S0_, double volatility_, double strike_, 
					   double maturity_, double divid_, double rfRate_, double barrier_)
					   :
					   S0(S0_), volatility(volatility_), strike(strike_),
					   maturity(maturity_), divid(divid_), rfRate(rfRate_), barrier(barrier_)
{
	numOfPaths = 10000; // default number of paths
	timeSteps = 252;    // default number of time steps per year
	se = 0;             // default standard error
}

/* return the standard error that saved for last simulation */
double EuroBarrier::getSE()
{
	return se;
}

/* closed-form solution */
double EuroBarrier::delta1(double tao, double s)
{
	double standardDeviation = volatility * sqrt(tao);
	return (log(s) + (rfRate-divid+0.5*volatility*volatility)*tao) / standardDeviation;
}

double EuroBarrier::delta2(double tao, double s)
{
	double standardDeviation = volatility * sqrt(tao);
	return (log(s) + (rfRate-divid-0.5*volatility*volatility)*tao) / standardDeviation;
}

double EuroBarrier::closeForm()
{
	double temp1 = pow(S0/barrier, (-(2.0*(rfRate-divid)/(volatility*volatility))-1));
	double temp2 = pow(S0/barrier, (-(2.0*(rfRate-divid)/(volatility*volatility))+1));
	double I1 = CumulativeNormal(delta1(maturity,S0/strike))-CumulativeNormal(delta1(maturity,S0/barrier));
	double I2 = exp(-rfRate*maturity) * (CumulativeNormal(delta2(maturity,S0/strike))-CumulativeNormal(delta2(maturity,S0/barrier)));
	double I3 = temp1 * (CumulativeNormal(delta1(maturity,(barrier*barrier)/(strike*S0)))-CumulativeNormal(delta1(maturity,barrier/S0)));
	double I4 = exp(-rfRate*maturity) * temp2 * (CumulativeNormal(delta2(maturity,(barrier*barrier)/(strike*S0)))-CumulativeNormal(delta2(maturity,barrier/S0)));
	return S0 * exp(-divid*maturity) * I1 - strike * I2 - S0 * exp(-divid*maturity) * I3 + strike * I4;
}

/* Euler numerical solution of SDE for spot */
double EuroBarrier::EulerCall()
{
	double a[numOfPaths];
	double result = 0.0;
	double mesh = 1.0 / timeSteps;
	double rootMesh = sqrt(mesh);
	double St = S0;
	double Mt = 0;  // maximum process
	for (int j = 0; j < numOfPaths; j++)
	{
		St = S0;
		Mt = S0;
		for (int i = 0; i < timeSteps; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			St = St + St*(rfRate-divid)*mesh + St*volatility*rootMesh*thisGaussian;
			Mt = St > Mt ? St : Mt;
		}
		double thisPayoff = 0;

		if (Mt > barrier)
			thisPayoff = 0;
		else
			thisPayoff = St - strike;

		thisPayoff = thisPayoff > 0 ? thisPayoff : 0;

		a[j] = thisPayoff;

		result += thisPayoff;
	}
	double mean = result / numOfPaths;

	double sse = 0;
	for (int i = 0; i < numOfPaths; i++)
	{
		sse += (a[i]-mean)*(a[i]-mean);
	}
	se = exp(-rfRate*maturity) * sqrt(sse/(numOfPaths-1)) / sqrt(numOfPaths);

	mean *= exp(-rfRate*maturity);
	return mean;
}

/* Use Antithetic variates method to calculate barrier option price */
double EuroBarrier::antitheticCall()
{
	double a[numOfPaths];
	double result = 0.0;
	double mesh = 1.0 / timeSteps;
	double rootMesh = sqrt(mesh);
	double St1 = S0;
	double St2 = S0;
	double Mt1 = 0, Mt2 = 0;  // maximum process
	for (int j = 0; j < numOfPaths; j++)
	{
		St1 = S0;
		St2 = S0;
		Mt1 = S0;
		Mt2 = S0;
		for (int i = 0; i < timeSteps; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			St1 = St1 + St1*(rfRate-divid)*mesh + St1*volatility*rootMesh*thisGaussian;
			St2 = St2 + St2*(rfRate-divid)*mesh - St2*volatility*rootMesh*thisGaussian; // use St1 or St2?
			Mt1 = St1 > Mt1 ? St1 : Mt1;
			Mt2 = St2 > Mt2 ? St2 : Mt2;
		}
		double thisPayoff1 = 0;
		double thisPayoff2 = 0;

		if (Mt1 > barrier)
			thisPayoff1 = 0;
		else
			thisPayoff1 = St1 - strike;

		thisPayoff1 = thisPayoff1 > 0 ? thisPayoff1 : 0;


		if (Mt2 > barrier)
			thisPayoff2 = 0;
		else
			thisPayoff2 = St2 - strike;

		thisPayoff2 = thisPayoff2 > 0 ? thisPayoff2 : 0;

		a[j] = (thisPayoff1 + thisPayoff2) / 2;

		result += (thisPayoff1 + thisPayoff2) / 2;
	}
	double mean = result / numOfPaths;

	double sse = 0;
	for (int i = 0; i < numOfPaths; i++)
	{
		sse += (a[i]-mean)*(a[i]-mean);
	}
	se = exp(-rfRate*maturity) * sqrt(sse/(numOfPaths-1)) / sqrt(numOfPaths);

	mean *= exp(-rfRate*maturity);
	return mean;
}

/* Use the Control variates method */
double EuroBarrier::controlCall()
{
	unsigned long n1 = numOfPaths/3;  // the number of simulations to estimate bstar
	unsigned long n2 = numOfPaths/3;  // the number of simulations to estimate E[Y]

	/* First stage: simulate n1 times to estimate bstar */
	double a[n1];
	double b[n1];
	double result = 0.0;
	double mesh = 1.0 / timeSteps;
	double rootMesh = sqrt(mesh);
	double St = S0;
	double Mt = 0;  // maximum process
	double Ysum = 0, Xsum = 0;
	for (int j = 0; j < n1; j++)
	{
		St = S0;
		Mt = S0;
		for (int i = 0; i < timeSteps; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			St = St + St*(rfRate-divid)*mesh + St*volatility*rootMesh*thisGaussian;
			Mt = St > Mt ? St : Mt;
		}
		double thisPayoffY = 0;
		double thisPayoffX = St - strike;

		if (Mt > barrier)
			thisPayoffY = 0;
		else
			thisPayoffY = St - strike;

		thisPayoffY = thisPayoffY > 0 ? thisPayoffY : 0;

		thisPayoffX = thisPayoffX > 0 ? thisPayoffX : 0;

		a[j] = thisPayoffY;
		b[j] = thisPayoffX;
		Ysum += thisPayoffY;
		Xsum += thisPayoffX;
	}
	/* estimate bstar */
	double Ybar = Ysum / n1;
	double Xbar = Xsum / n1;
	double cov = 0, var = 0;
	double sigmaY2 = 0;
	for (int i = 0; i < n1; i++)
	{
		cov += (b[i]-Xbar)*(a[i]-Ybar);
		var += (b[i]-Xbar)*(b[i]-Xbar);
		sigmaY2 += (a[i]-Ybar)*(a[i]-Ybar);
	}
	double bstar = cov / var;

	/* print out bstar and rho to answer the question (comment here before submission) */
	// cout << "Bstar = " << bstar << endl; // print out bstar to answer the question
	// double rho = bstar * sqrt(var) / sqrt(sigmaY2);
	// cout << "rho = " << rho << endl;



	/* Second stage: simulate n2 times to estimate E[Y] */
	double y[n2];
	double EX = BSCall() * exp(rfRate*maturity);
	for (int j = 0; j < n2; j++)
	{
		St = S0;
		Mt = S0;
		for (int i = 0; i < timeSteps; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			St = St + St*(rfRate-divid)*mesh + St*volatility*rootMesh*thisGaussian;
			Mt = St > Mt ? St : Mt;
		}
		double thisPayoffY = 0;
		double thisPayoffX = St - strike;

		if (Mt > barrier)
			thisPayoffY = 0;
		else
			thisPayoffY = St - strike;

		thisPayoffY = thisPayoffY > 0 ? thisPayoffY : 0;

		thisPayoffX = thisPayoffX > 0 ? thisPayoffX : 0;

		y[j] = thisPayoffY - bstar*(thisPayoffX-EX);
		result += y[j];
	}


	double mean = result / n2;

	double sse = 0;
	for (int i = 0; i < n2; i++)
	{
		sse += (y[i]-mean)*(y[i]-mean);
	}
	se = exp(-rfRate*maturity) * sqrt(sse/(n2-1)) / sqrt(n2);

	mean *= exp(-rfRate*maturity);
	return mean;

}

/* Use BSM formula to give closed-form solution of a vanilla call option */
double EuroBarrier::BSCall()
{
	double standardDeviation = volatility * sqrt(maturity);
	double moneyness = log(S0/strike);
	double d1 = (moneyness + (rfRate-divid)*maturity + 0.5*standardDeviation*standardDeviation) / standardDeviation;
	double d2 = d1 - standardDeviation;
	return S0 * exp(-divid*maturity) * CumulativeNormal(d1) - strike * exp(-rfRate*maturity) * CumulativeNormal(d2);

}


/* Use the Control variates method to estimate a vanilla call option (to answer the second question) */
double EuroBarrier::controlVanillaCall()
{
	unsigned long n1 = numOfPaths/3;  // the number of simulations to estimate bstar
	unsigned long n2 = numOfPaths/3;  // the number of simulations to estimate E[Y]

	/* First stage: simulate n1 times to estimate bstar */
	double a[n1];
	double b[n1];
	double result = 0.0;
	double mesh = 1.0 / timeSteps;
	double rootMesh = sqrt(mesh);
	double St = S0;
	double Ysum = 0, Xsum = 0;
	for (int j = 0; j < n1; j++)
	{
		St = S0;
		for (int i = 0; i < timeSteps; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			St = St + St*(rfRate-divid)*mesh + St*volatility*rootMesh*thisGaussian;
		}
		double thisPayoffY = 0;
		double thisPayoffX = St - strike;

		thisPayoffY = St;

		thisPayoffX = thisPayoffX > 0 ? thisPayoffX : 0;

		a[j] = thisPayoffY;
		b[j] = thisPayoffX;
		Ysum += thisPayoffY;
		Xsum += thisPayoffX;
	}
	/* estimate bstar */
	double Ybar = Ysum / n1;
	double Xbar = Xsum / n1;
	double cov = 0, var = 0;
	double sigmaY2 = 0;
	for (int i = 0; i < n1; i++)
	{
		cov += (b[i]-Xbar)*(a[i]-Ybar);
		var += (b[i]-Xbar)*(b[i]-Xbar);
		sigmaY2 += (a[i]-Ybar)*(a[i]-Ybar);
	}
	double bstar = cov / sigmaY2;

	/* print out bstar and rho to answer the question */
	// cout << "Bstar = " << bstar << endl; // print out bstar to answer the question
	// double rho = bstar * sqrt(sigmaY2) / sqrt(var);
	// cout << "rho = " << rho << endl;


	/* Second stage: simulate n2 times to estimate E[Y] */
	double y[n2];
	double EX = Ybar;
	for (int j = 0; j < n2; j++)
	{
		St = S0;
		for (int i = 0; i < timeSteps; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			St = St + St*(rfRate-divid)*mesh + St*volatility*rootMesh*thisGaussian;
		}
		double thisPayoffY = 0;
		double thisPayoffX = St;
		thisPayoffY = St - strike;

		thisPayoffY = thisPayoffY > 0 ? thisPayoffY : 0;

		y[j] = thisPayoffY - bstar*(thisPayoffX-EX);
		result += y[j];
	}

	double mean = result / n2;

	double sse = 0;
	for (int i = 0; i < n2; i++)
	{
		sse += (y[i]-mean)*(y[i]-mean);
	}
	se = exp(-rfRate*maturity) * sqrt(sse/(n2-1)) / sqrt(n2);

	mean *= exp(-rfRate*maturity);
	return mean;

}



