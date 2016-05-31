#include "stdafx.h"

#include "Enemy.h"

#include <iostream>
#include "RandomGenerator.h"
using namespace std;

Enemy::Enemy(int lvl) : EnemyBase()
{
	level = lvl;
	RandomGenerator rg = RandomGenerator();
	name = EnemyName[rg.getRandom(0,5)];
	state = EnemyState[rg.getRandom(0, 5)];
	health = rg.getRandom(2, 6);
	alive = true;
}


Enemy::~Enemy()
{
}


