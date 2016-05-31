#pragma once
#include <string>
const std::string  EquipmentName[]{ "Sword", "broom", "gun", "pen", "lighter", "table"};
const std::string  EquipmentSize[]{ "huge", "gigantic", "small", "mini", "big", "medium" };
const std::string  EquipmentColor[]{ "Red", "Blue", "Green", "Gold", "Silver", "Orange" };
class Equipment
{
	

public:
	std::string name;
	std::string size;
	std::string color;
	int minimumLevel;
	int hitpoints;
	bool found = false;
	
	Equipment();
	~Equipment();
	std::string getName();

	void setFound();
	bool isFound();
	int getAwarenes();
};

