// StandNorm class
#ifndef STANDNORM_H
#define STANDNORM_H

#include "Random1.h" //BoxMuller
#include "ParkMiller.h"
#include "LEcuyer.h"
#include "Normals.h"


class StandNorm
{
public:
	StandNorm();
	
	void BoxMullerParkMiller();
    void BoxMullerLEcuyer();
    
    void FishmanParkMiller();
    void FishmanLEcuyer();
    
    void InversParkMiller();
    void InversLEcuyer();
    
private:
    unsigned long numOfPaths = 1000;
};

#endif