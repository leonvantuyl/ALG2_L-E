#pragma once
#include <string>
#include <vector>
#include "Equipment.h"
class Player
{
public:
	std::string name;
	int level = 0;
	int xperience = 50;
	int health = 80;
	std::vector<Equipment> inventory;
	Player(std::string);
	~Player();
	void showStats();
	void addEquipment(Equipment);
	void addXP(int id);
	int getAttack();
	int getExperience();
	int getHealth();
	int getDefence();
	int getLevel();
	int getAwarenes();
	int doAttack();
	int getMaxHealth();
	void takeDamage(int);
	void rest();
	bool isAlive();
};

