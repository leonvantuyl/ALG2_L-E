#include "stdafx.h"
#include "Boss.h"
#include "RandomGenerator.h"

Boss::Boss(int lvl)
{

	RandomGenerator rg;
	level = lvl;
	name = BossName[rg.getRandom(0,5)];
	state = "Furious";
	health = 10 + rg.getRandom( 100, 150 )*(lvl + 1);

	attackpoints = rg.getRandom( 5, 8 )* (lvl + 1);
}


Boss::~Boss()
{
}
