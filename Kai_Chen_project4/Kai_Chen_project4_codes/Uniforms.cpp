// functions for class Uniforms

#include "Uniforms.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>

using namespace std;

Uniforms::Uniforms()
{
    
}

void Uniforms::ParkMillerUniforms()
{

	double x;
	double y;

	ParkMiller generator(1);

	for (int i = 0; i < numOfPaths; i++)
	{

		x = 1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min());
		y = 1.0 * generator.GetOneRandomInteger() / (generator.Max()-generator.Min());

        // write the data into a txt file
        ofstream file;
        file.open("/Users/chenkai/Desktop/UniformsParkMiller.txt", ios::app);
        file << x << "\t" << y << "\n";
		//cout << result << endl;
	}
}

void Uniforms::SobolUniforms()
{
	int seed = 123456789;
	double x, y;
	for (int i = 0; i < numOfPaths; i++)
	{

		x = 1.0 * i4_uniform ( 2, 100, &seed ) / 100; // 2-dimensional sobol sequence
		y = 1.0 * i4_uniform ( 2, 100, &seed ) / 100;

        // write the data into a txt file
        ofstream file;
        file.open("/Users/chenkai/Desktop/UniformsSobol.txt", ios::app);
        file << x << "\t" << y << "\n";
	}
}





