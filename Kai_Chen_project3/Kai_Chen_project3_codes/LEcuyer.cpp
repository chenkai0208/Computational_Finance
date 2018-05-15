//
//
//                       LEcuyer.cpp
//
//

#include "LEcuyer.h"

const long m1 = 2147483647;
const long a11 = 0;
const long a12 = 63308;
const long a13 = -183326;
const long m2 = 2145483479;
const long a21 = 86098;
const long a22 = 0;
const long a23 = -539608;

const long q12 = m1/a12;
const long q13 = m1/a13;
const long q21 = m2/a21;
const long q23 = m2/a23;

const long r12 = m1 - a12*q12;
const long r13 = m1 - a13*q13;
const long r21 = m2 - a21*q21;
const long r23 = m2 - a23*q23;

LEcuyer::LEcuyer(long Seed_) : Seed(Seed_)
{
    if (Seed == 0)
        Seed = 1;
	x10 = 1;
	x11 = 2;
	x12 = 3;
	x20 = 5;
	x21 = 7;
	x22 = 11;
}

void LEcuyer::SetSeed(long Seed_)
{
    Seed = Seed_;
    if (Seed == 0)
        Seed = 1;
	x10 = 1;
	x11 = 2;
	x12 = 3;
	x20 = 5;
	x21 = 7;
	x22 = 11;
}

unsigned long LEcuyer::Max()
{
	return m1-1; 
}

unsigned long LEcuyer::Min()
{
	return 1;
}

long LEcuyer::GetOneRandomInteger()
{
	long h = 0;
	h = x11/q12;
	long p12 = a12 * (x11-h*q12) - h*r12;
	if (p12 < 0)
		p12 = p12 + m1;
	h = x10/q13;
	long p13 = a13 * (x10-h*q13) - h*r13;
	if (p13 < 0)
		p13 = p13 + m1;
	h = x22/q21;
	long p21 = a21 * (x22-h*q21) - h*r21;
	if (p21 < 0)
		p21 = p21 + m2;
	h = x20/q23;
	long p23 = a23 * (x20-h*q23) - h*r23;
	if (p23 < 0)
		p23 = p23 + m2;
	x10 = x11;
	x11 = x12;
	p12 = p12 - m1;
	x12 = p12 + p13;
	if (x12 < 0)
		x12 = x12 + m1;
	x20 = x21;
	x21 = x22;
	p21 = p21 - m2;
	x22 = p21 + p23;
	if (x22 < 0)
		x22 = x22 + m2;
	long x1 = 0; // initialize x1
	if (x12 < x22)
		x1 = x12 - x22 + m1;
	else
		x1 = x12 - x22;
	if (x1 == 0)
		x1 = m1;
	double k = 1 / (1.0 + m1);
    //Seed = x1 * k;
	return x1;
}

RandomLEcuyer::RandomLEcuyer(unsigned long Dimensionality, unsigned long Seed)
	:	RandomBase(Dimensionality),
		InnerGenerator(Seed),
		InitialSeed(Seed)
{
	Reciprocal = 1 / (1.0+InnerGenerator.Max());
}

RandomBase* RandomLEcuyer::clone() const
{
	return new RandomLEcuyer(*this);
}

void RandomLEcuyer::GetUniforms(MJArray& variates)
{
    for (unsigned long j=0; j < GetDimensionality(); j++)
        variates[j] = InnerGenerator.GetOneRandomInteger()*Reciprocal;
}

void RandomLEcuyer::Skip(unsigned long numberOfPaths)
{
    MJArray tmp(GetDimensionality());
    for (unsigned long j=0; j < numberOfPaths; j++)
        GetUniforms(tmp);
}

void RandomLEcuyer::SetSeed(unsigned long Seed)
{
    InitialSeed = Seed;
    InitialSeed1 = 1;
    InitialSeed2 = 2;
    InitialSeed3 = 3;
    InitialSeed4 = 5;
    InitialSeed5 = 7;
    InitialSeed6 = 11;
    InnerGenerator.SetSeed(Seed);
}

void RandomLEcuyer::Reset()
{
    InnerGenerator.SetSeed(InitialSeed);
}

void RandomLEcuyer::ResetDimensionality(unsigned long NewDimensionality)
{
    RandomBase::ResetDimensionality(NewDimensionality);
    InnerGenerator.SetSeed(InitialSeed);
}

