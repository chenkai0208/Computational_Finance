// Uniforms class
#ifndef UNIFORMS_H
#define UNIFORMS_H

#include "ParkMiller.h"
#include "SobolGen.h"
#include "Normals.h"
#include "sobol.hpp"


class Uniforms
{
public:
	Uniforms();
	
	void ParkMillerUniforms();
	void SobolUniforms();
    
private:
    unsigned long numOfPaths = 1024;
};

#endif