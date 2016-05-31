#include "stdafx.h"

#include "Enemy.h"

#include <iostream>
#include "RandomGenerator.h"
using namespace std;

Enemy::Enemy(int lvl) : EnemyBase()
{
	level = lvl;
	RandomGenerator rg;
	name = EnemyName[rg.getRandom(0,5)];
	state = EnemyState[rg.getRandom(0, 5)];
	health = rg.getRandom(10, 20) * (lvl +1);
	alive = true;
	attackpoints = rg.getRandom( 2, 6 ) * (lvl + 1);
}


Enemy::~Enemy()
{
}


