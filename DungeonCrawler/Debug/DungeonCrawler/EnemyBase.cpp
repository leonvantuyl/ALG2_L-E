#include "stdafx.h"
#include "EnemyBase.h"

#include <iostream>

EnemyBase::EnemyBase()
{
}


EnemyBase::~EnemyBase()
{
}

int attack()
{
	return 0;
}
int EnemyBase::getHealth(){
	return health;
}
std::string EnemyBase::getDescription()
{
	return state + " " + name;
}

bool EnemyBase::attackMe(int dmg)
{
	health -= dmg;
	if (health < 1)
	{
		alive = false;
		std::cout << "You killed an enemy" << std::endl;
	}
	else
	{
		std::cout << "Enemy is still alive " << health << " health left" << std::endl;

	}
	return alive;
}

bool EnemyBase::isAlive(){
	return (health > 0);
}