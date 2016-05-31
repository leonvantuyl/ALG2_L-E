	#include "stdafx.h"
	#include "RandomGenerator.h"

	RandomGenerator::RandomGenerator()
	{	
		int x = 0;
	}


	RandomGenerator::~RandomGenerator()
	{
	}

	int RandomGenerator::getRandom(int min, int max)
	{
		std::uniform_int_distribution<int> distSize{ min, max };
		int returnVal = distSize(dev);
		return returnVal;
	}