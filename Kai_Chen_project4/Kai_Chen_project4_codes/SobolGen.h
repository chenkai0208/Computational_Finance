//
//
//                      SobolGen.h
//
//
#ifndef SOBOLGEN
#define SOBOLGEN
#include "Random2.h"
#include "sobol.hpp"


class SobolGen
{
public:
    
    SobolGen(long Seed = 1);

    //long GetOneRandomInteger();
    void SetSeed(long Seed);

    static unsigned long Max();
    static unsigned long Min();

private:

    long Seed;

};

class RandomSobolGen : public RandomBase
{
public:

    RandomSobolGen(unsigned long Dimensionality, unsigned long Seed=1);

    virtual RandomBase* clone() const;
    virtual void GetUniforms(MJArray& variates);    
    virtual void Skip(unsigned long numberOfPaths);
    virtual void SetSeed(unsigned long Seed);
    virtual void Reset();
    virtual void ResetDimensionality(unsigned long NewDimensionality);

private:

    SobolGen InnerGenerator;
    unsigned long InitialSeed;
    double Reciprocal;

};
#endif


