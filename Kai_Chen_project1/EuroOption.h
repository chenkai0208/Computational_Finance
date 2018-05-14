// EuroOption class

#ifndef EUROOPTION_H
#define EUROOPTION_H
class EuroOption
{
public:
	EuroOption(double, double, double, double, double, double);

	double BSCall();              // use Black Scholes formula as the closed-form solution
	double singleStepExactCall(); // use single-step exact SDE solution
	double EulerCall1();          // use Euler numerical solution of SDE for spot
	double EulerCall2();          // use Euler numerical solution of SDE for log spot
	double MilsteinCall();        // use Milstein numerical solution of SDE for spot

	double BSPut();              // use Black Scholes formula as the closed-form solution
	double singleStepExactPut(); // use single-step exact SDE solution
	double EulerPut1();          // use Euler numerical solution of SDE for spot
	double EulerPut2();          // use Euler numerical solution of SDE for log spot
	double MilsteinPut();        // use Milstein numerical solution of SDE for spot

	/* Error Analysis */
	double singleStepExactError();
	double Euler1Error();
	double Euler2Error();
	double MilsteinError();
private:
	double S0; // intitial stock price
	double volatility; // volatility of the underlying
	double strike; // strick price
	double maturity; // maturity of the option
	double divid; // dividend yied
	double rfRate; // risk-free rate

	/* Variables for Monte Carlo */
	unsigned long numOfPaths; // the number of generated paths
	unsigned long timeSteps;  // the number of time steps per year
}; // end class

#endif