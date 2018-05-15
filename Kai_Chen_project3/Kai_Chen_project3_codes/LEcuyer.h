//
//
//                       LEcuyer.h
//
//

#ifndef LECUYER_H
#define LECUYER_H

#include "Random2.h"

class LEcuyer
{
public:
	LEcuyer(long Seed = 1);
	long GetOneRandomInteger();

    void SetSeed(long Seed);

    static unsigned long Max();
    static unsigned long Min();

private:
    long Seed;
	long x10 = 1;
	long x11 = 2;
	long x12 = 3;
	long x20 = 5;
	long x21 = 7;
	long x22 = 11;
};

class RandomLEcuyer : public RandomBase
{
public:
	RandomLEcuyer(unsigned long Dimensionality, unsigned long Seed=1);
    virtual RandomBase* clone() const;
    virtual void GetUniforms(MJArray& variates);    
    virtual void Skip(unsigned long numberOfPaths);
    virtual void SetSeed(unsigned long Seed);
    virtual void Reset();
    virtual void ResetDimensionality(unsigned long NewDimensionality);

private:
    LEcuyer InnerGenerator;
    unsigned long InitialSeed;
    long InitialSeed1;
    long InitialSeed2;
    long InitialSeed3;
    long InitialSeed4;
    long InitialSeed5;
    long InitialSeed6;
    double Reciprocal;
};

#endif