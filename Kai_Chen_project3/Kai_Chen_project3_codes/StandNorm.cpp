// member functions for class StandNorm

#include "StandNorm.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>

using namespace std;

StandNorm::StandNorm()
{
    
}

void StandNorm::BoxMullerParkMiller()
{
	double result;

	double x;
	double y;

	ParkMiller generator(1);

	double sizeSquared;
	for (int i = 0; i < numOfPaths; i++)
	{
		do
		{
			x = 2.0*generator.GetOneRandomInteger() / (generator.Max()-generator.Min())-1;
			y = 2.0*generator.GetOneRandomInteger() / (generator.Max()-generator.Min())-1;
			sizeSquared = x*x + y*y;
	        //cout << x << " " << y << endl;
		}
		while
			( sizeSquared >= 1.0);

		result = x*sqrt(-2*log(sizeSquared)/sizeSquared);
        // write the data into a txt file
        ofstream file;
        file.open("/Users/chenkai/Desktop/BoxMullerParkMiller.txt", ios::app);
        file << result << "\n";
		//cout << result << endl;
	}
}

void StandNorm::BoxMullerLEcuyer()
{
    double result;
    
    double x;
    double y;
    
    LEcuyer generator(1);
    
    double sizeSquared;
    for (int i = 0; i < numOfPaths; i++)
    {
        do
        {
            x = 2.0*generator.GetOneRandomInteger() / (generator.Max()-generator.Min())-1;
            y = 2.0*generator.GetOneRandomInteger() / (generator.Max()-generator.Min())-1;
            sizeSquared = x*x + y*y;
            //cout << x << " " << y << endl;
        }
        while
            ( sizeSquared >= 1.0);
        
        result = x*sqrt(-2*log(sizeSquared)/sizeSquared);
        // write the data into a txt file
        ofstream file;
        file.open("/Users/chenkai/Desktop/BoxMullerLEcuyer.txt", ios::app);
        file << result << "\n";
        //cout << result << endl;
    }
}

void StandNorm::FishmanParkMiller()
{
    double result;
    double U1, U2, U3, test;
    ParkMiller generator(1);
    
    for (int i = 0; i < numOfPaths; i++)
    {
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
        // write the data into a txt file
        ofstream file;
        file.open("/Users/chenkai/Desktop/FishmanParkMiller.txt", ios::app);
        file << result << "\n";
    }
}

void StandNorm::FishmanLEcuyer()
{
    double result;
    double U1, U2, U3, test;
    LEcuyer generator(1);
    
    for (int i = 0; i < numOfPaths; i++)
    {
        test = rand()/static_cast<double>(RAND_MAX)-1;
        U1 = 1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min());
        U2 = 1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min());
        U3 = 1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min());
        double X = -log(U1);
        while (U2 > exp(-0.5*(X-1)*(X-1)))  //U2 > exp(-0.5*(X-1)*(X-1))
        {
            test = rand()/static_cast<double>(RAND_MAX)-1;
            //U1 = rand()/static_cast<double>(RAND_MAX);
            //U2 = rand()/static_cast<double>(RAND_MAX);
            //U3 = rand()/static_cast<double>(RAND_MAX);
            U1 = 1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min());
            U2 = 1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min());
            U3 = 1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min());
            X = -log(U1);
        }
        
        if (U3 <= 0.5)
            X = -X;
        result = X;
        // write the data into a txt file
        ofstream file;
        file.open("/Users/chenkai/Desktop/FishmanLEcuyer.txt", ios::app);
        file << result << "\n";
    }
}

void StandNorm::InversParkMiller()
{
    double result;
    ParkMiller generator(1);
    for (int i = 0; i < numOfPaths; i++)
    {
        result = InverseCumulativeNormal(1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min()));
        // write the data into a txt file
        ofstream file;
        file.open("/Users/chenkai/Desktop/InversParkMiller.txt", ios::app);
        file << result << "\n";
    }
}

void StandNorm::InversLEcuyer()
{
    double result;
    LEcuyer generator(1);
    for (int i = 0; i < numOfPaths; i++)
    {
        result = InverseCumulativeNormal(1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min()));
        // write the data into a txt file
        ofstream file;
        file.open("/Users/chenkai/Desktop/InversLEcuyer.txt", ios::app);
        file << result << "\n";
    }
}


