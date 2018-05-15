using namespace std;
#include <vector>

#ifndef PSOR_H
#define PSOR_H

class PSOR
{
private:
	double w; // control parameter for PSOR algorithm
	vector<vector<double>> A; // lhs of system
	vector<double> F; // rhs of system
	vector<double> u; // solution to the system
	double epsilon = 10e-5; // error tolerance for a single step
public:
	PSOR(vector<vector<double>>, vector<double>, vector<double>, double);

	vector<double> solve(vector<vector<double>> A, vector<double> F, vector<double> u, double w);
	void print();

};

#endif