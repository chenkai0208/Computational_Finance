//
//
//                       SobolGen.cpp
//
//

#include "SobolGen.h"
#include "sobol.hpp"

const long a = 16807;
const long m = 2147483647;
const long q = 127773;
const long r = 2836;
int seed = 123456789;

SobolGen::SobolGen(long Seed_ ) : Seed(Seed_)
{
    if (Seed ==0)
        Seed=1;
}

  
void SobolGen::SetSeed(long Seed_)
{
  Seed=Seed_;
  if (Seed ==0)
        Seed=1;
}

unsigned long SobolGen::Max()
{
    return m-1;
}

unsigned long SobolGen::Min()
{
    return 1;
}
/*
long SobolGen::GetOneRandomInteger()
{
    long k;
 
    k=Seed/q;
    
    Seed=a*(Seed-k*q)-r*k; 

    if (Seed < 0) 
        Seed += m;

    return Seed;
}
*/

RandomSobolGen::RandomSobolGen(unsigned long Dimensionality, unsigned long Seed)
:    RandomBase(Dimensionality),
    InnerGenerator(Seed),
    InitialSeed(Seed)
{
    Reciprocal = 1/(1.0+InnerGenerator.Max());
}

RandomBase* RandomSobolGen::clone() const
{
    return new RandomSobolGen(*this);   
}

void RandomSobolGen::GetUniforms(MJArray& variates)
{
    for (unsigned long j=0; j < GetDimensionality(); j++)
        variates[j] = i4_uniform(GetDimensionality(), 100, &seed)*Reciprocal;

}
    
void RandomSobolGen::Skip(unsigned long numberOfPaths)
{
    MJArray tmp(GetDimensionality());
    for (unsigned long j=0; j < numberOfPaths; j++)
        GetUniforms(tmp);
}

void RandomSobolGen::SetSeed(unsigned long Seed)
{
    InitialSeed = Seed;
    InnerGenerator.SetSeed(Seed);
}

void RandomSobolGen::Reset()
{
    InnerGenerator.SetSeed(InitialSeed);
}


void RandomSobolGen::ResetDimensionality(unsigned long NewDimensionality)
{
    RandomBase::ResetDimensionality(NewDimensionality);
    InnerGenerator.SetSeed(InitialSeed);
}


