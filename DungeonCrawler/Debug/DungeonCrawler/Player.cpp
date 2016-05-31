#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include "RandomGenerator.h"
Player::Player(std::string pName)
{
	name = pName;
}




Player::~Player()
{
}

void Player::showStats()
{
	std::cout << "----  STATS OF ---" << name << std::endl;
	std::cout << "Level :" << getLevel() << std::endl;
	std::cout << "Experience :" << getExperience() << std::endl;
	std::cout << "Health :" << getHealth() << std::endl;
	std::cout << "Attack :" << getAttack() << std::endl;
	std::cout << "Defence :" << getDefence() << std::endl;
	std::cout << "Awareness :" << getAwarenes() << std::endl << std::endl;
	std::cout << "Inventory :" << getAwarenes() << std::endl << std::endl;
	for (int i = 0; i < inventory.size(); i++)
	{
		std::cout << i << ": " << inventory.at(i).getName() << std::endl;
	}
	std::cout << std::endl;
}

void Player::addEquipment(Equipment nEquipment)
{
	inventory.push_back(nEquipment);
}

int Player::getAttack()
{
	return xperience / 5;
}
int Player::getExperience()
{
	return xperience;
}

int Player::doAttack()
{
	RandomGenerator rg;
	int retunVal = rg.getRandom( 0, getAttack());
	return retunVal;
}

void Player::addXP(int newXP)
{
	xperience += newXP;
}

int Player::getMaxHealth()
{
	 return 100 + level * 10;
}

int Player::getHealth()
{
	return health;
}
int Player::getDefence()
{
	return xperience / 5;
}
int Player::getLevel()
{
	return xperience / 50;
}
int Player::getAwarenes()
{
	return xperience / 5;
}

void Player::takeDamage(int dmg)
{
	health -= dmg;
}

void Player::rest()
{
	health += 10;
	if (health > getMaxHealth())
	{
		health = getMaxHealth();
		std::cout << "Your health is full !" << std::endl;
	}
	else
		std::cout << "Your health is increased" << std::endl;

}

bool Player::isAlive()
{
	if (health > 0)
		return true;
	return false;
}