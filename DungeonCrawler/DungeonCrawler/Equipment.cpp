#include "stdafx.h"
#include "Equipment.h"
#include "RandomGenerator.h"
#include <sstream>

Equipment::Equipment()
{
	RandomGenerator rg;
	hitpoints = rg.getRandom(2, 17);
	color  = EquipmentColor[rg.getRandom(0, 5)];
	name = EquipmentName[rg.getRandom(0, 5)];
	size = EquipmentSize[rg.getRandom(0, 5)];
	minimumLevel = rg.getRandom(0, 100);
}


Equipment::~Equipment()
{
}

int Equipment::getHitpoints()
{
	return hitpoints;
}

std::string Equipment::getName()
{
	std::ostringstream os;
	os << "a " << size  + " " + color + " " + name + " damage " << hitpoints << " (" << minimumLevel << ") "<< ".";
	return os.str();
}

void Equipment::setFound()
{
	found = true;
}

bool Equipment::isFound()
{
	return found;
}

int Equipment::getAwarenes()
{
	return minimumLevel;
}