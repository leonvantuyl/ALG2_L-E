#pragma once
#include "EnemyBase.h"
class Boss : EnemyBase
{
	int level;
public:
	Boss(int lvl);
	~Boss();
	void Attack();
};

