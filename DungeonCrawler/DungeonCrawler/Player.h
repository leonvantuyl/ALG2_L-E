#pragma once
#include <string>
#include <vector>
#include "Equipment.h"
const std::string  playerName[]
{
	"Henk", "Rambo", "Lionell Mesi", "Bruce Lee", "Wesley snipes", "Steven Seagal"
};
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
	std::string getName();
	int getLevel();
	int getAwarenes();
	int doAttack();
	int getMaxHealth();
	void takeDamage(int);
	void printInventory();
	Equipment* getEquipment(size_t);
	void rest();
	bool isAlive();
};

