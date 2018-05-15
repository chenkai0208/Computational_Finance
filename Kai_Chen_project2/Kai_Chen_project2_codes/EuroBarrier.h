// EuroBarrier class

#ifndef EUROBARRIER_H
#define EUROBARRIER_H
class EuroBarrier
{
public:
	EuroBarrier(double, double, double, double, double, double, double); // constructor
	double delta1(double, double); // positive delta for closed-form formula
	double delta2(double, double); // negative delta for closed-form formula
	double closeForm();   // Closed-form barrier option price
	double EulerCall();   // Euler numerical solution of SDE for spot
	double antitheticCall(); // Use Antithetic variates method to calculate barrier option price
	double controlCall();    // Use the Control variates method

	double BSCall();        // To give closed-form solution of a vanilla call option

	double getSE();         // get the current saved standard error of Monte Carlo simulation

	double controlVanillaCall();
private:
	double S0; // intitial stock price
	double volatility; // volatility of the underlying
	double strike; // strick price
	double maturity; // maturity of the option
	double divid; // dividend yied
	double rfRate; // risk-free rate

	double barrier; // barrier for the option

	/* Variables for Monte Carlo */
	unsigned long numOfPaths; // the number of generated paths
	unsigned long timeSteps;  // the number of time steps per year
	double se;                // the standard error for simulation

}; // end class


#endif