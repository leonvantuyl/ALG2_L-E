#pragma once
#include <string>
#include "EnemyBase.h"
#include <vector>

const std::string  EnemyName[]{ "goblin", "giant", "dwarf", "old guy", "nerd", "hipster"};
const std::string  EnemyState[]{ "lazy", "strong", "fast", "dirty", "weird", "drugged" };

class Enemy : public EnemyBase
{
	int level;
public:
	Enemy(int level);
	~Enemy();
};

