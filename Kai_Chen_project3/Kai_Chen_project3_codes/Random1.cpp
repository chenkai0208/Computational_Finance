//
//
//
//       				Random1.cpp
//
//           

//#include <Random1.h>
#include "Random1.h"
#include "Normals.h"
#include <cstdlib>
#include <cmath>
#include <time.h>
#include"iostream"
#include "ParkMiller.h"

// the basic math functions should be in namespace std but aren't in VCPP6
#if !defined(_MSC_VER)
using namespace std;
#endif

double GetOneGaussianBySummation()
{
	double result=0;

	for (unsigned long j=0; j < 12; j++)
		result += rand()/static_cast<double>(RAND_MAX);

	result -= 6.0;

	return result;

}


double GetOneGaussianByBoxMuller()
{
	double result;

	double x;
	double y;

	double sizeSquared;
	do
	{
		x = 2.0*rand()/static_cast<double>(RAND_MAX)-1;
		y = 2.0*rand()/static_cast<double>(RAND_MAX)-1;
		sizeSquared = x*x + y*y;
        //cout << x << " " << y << endl;
	}
	while
		( sizeSquared >= 1.0);

	result = x*sqrt(-2*log(sizeSquared)/sizeSquared);

	return result;

}

double GetOneGaussianByFishman()
{
    double result;
    double U1, U2, U3, test;
    ParkMiller generator(1);
    test = rand()/static_cast<double>(RAND_MAX)-1;
    U1 = 1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min());
    U2 = 1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min());
    U3 = 1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min());
    double X = -log(U1);
    while (U2 > exp(-0.5*(X-1)*(X-1)))  //U2 > exp(-0.5*(X-1)*(X-1))
    {
        test = rand()/static_cast<double>(RAND_MAX)-1;
        U1 = rand()/static_cast<double>(RAND_MAX);
        U2 = rand()/static_cast<double>(RAND_MAX);
        U3 = rand()/static_cast<double>(RAND_MAX);
        X = -log(U1);
    }
    
    if (U3 <= 0.5)
        X = -X;
    result = X;
    return result;

}

double GetOneGaussianByInverseDist()
{
    double result;
    double normal = rand()/static_cast<double>(RAND_MAX);
    result = InverseCumulativeNormal(normal);
    return result;
    
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

