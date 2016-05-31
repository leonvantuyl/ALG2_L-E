#pragma once
#include "EnemyBase.h"

const std::string  BossName[]
{
	"Dr doolitle", "Captain hook", "Nijntje", "Basilisk", "Stanley", "Nullpointer"
};
class Boss : public EnemyBase
{
	int level;
public:
	Boss(int lvl);
	~Boss();
	void Attack();
};

