#pragma once
#include <string>
#include <vector>
#include "Equipment.h"
class EnemyBase
{
	
public:
	std::string name;
	std::string state;
	int health;
	int attackpoints;
	bool alive = true;
	int attack();
	int getHealth();
	bool attackMe(int);
	bool isAlive();
	int getXP();
	std:: vector<Equipment> equipment;
	EnemyBase();
	~EnemyBase();
	std::string getDescription();
};

