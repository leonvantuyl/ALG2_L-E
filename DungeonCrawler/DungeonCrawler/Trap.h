#pragma once
#include <string>
const std::string  TrapDescription[]{ "tripwire", "landmine", "trap door", "spike", "hidden gun", "nuke"};
class Trap
{
	bool found = false;
	int damage;
	int minimalLvl;
	std::string description;
public:
	Trap();
	int getMinimalLevel();
	void setFound();
	~Trap();
	bool getFound();
	int getDamage();
	std::string getDescription();
};

