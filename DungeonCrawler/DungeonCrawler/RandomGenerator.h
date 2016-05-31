	#pragma once
	#include "stdafx.h"
	#include <random>

	class RandomGenerator
	{		
		std::random_device dev;
		std::default_random_engine dre{ dev() };
	public:
		RandomGenerator();
		~RandomGenerator();
		int getRandom(int , int);
	};

