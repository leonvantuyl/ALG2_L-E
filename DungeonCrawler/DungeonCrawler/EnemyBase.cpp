#include "stdafx.h"
#include "EnemyBase.h"

#include <iostream>

EnemyBase::EnemyBase()
{
}


EnemyBase::~EnemyBase()
{
}

int EnemyBase::attack()
{
	return attackpoints;
}
int EnemyBase::getHealth(){
	return health;
}
std::string EnemyBase::getDescription()
{
	std::string strAlive = " (Alive )";
	if( !isAlive() )
		strAlive = " (Dead) ";
	return state + " " + name + strAlive;
}

int EnemyBase::getXP()
{
	return attackpoints / 2;
}

void EnemyBase::killEnemy()
{
	health = -1;
	alive = false;
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

