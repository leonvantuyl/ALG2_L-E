#include "stdafx.h"
#include "Room.h"
#include <iostream>
#include "Boss.h"
#include "RandomGenerator.h"

Room::Room()
{
	Boss endBoss = Boss(10);
	RandomGenerator rg = RandomGenerator();
	size = RoomSize[rg.getRandom(0, 2)];
	content = RoomContent[rg.getRandom(0, 3)];
	description = RoomDescription[rg.getRandom(0, 3)];
	appearance = RoomAppearance[rg.getRandom(0, 3)];
	lighting = RoomLighting[rg.getRandom(0,3)];
	connected[RoomDirection::NORTH] =  nullptr;
	connected[RoomDirection::EAST] = nullptr;
	connected[RoomDirection::SOUTH] = nullptr;
	connected[RoomDirection::WEST] = nullptr;
	visited = false;
	visiting = false;
	exit = false;
	equipment.push_back(Equipment());
}


Room::~Room()
{
}

string Room::getSymbolCheat(){
	if (visiting)
		return "P";
	else if (visited)
		return "X";
	else
		return "x";
}

string Room::getSymbol(){
	if (visiting)
		return "P";
	else if (visited)
		return "X";
	else 
		return ".";
	
}

bool Room::getVisited()
{
	return visited;
}

void Room::setVisiting(bool isVisiting)
{
	visiting = isVisiting;
}

void Room::setVisited()
{
	visited = true;
}

void Room::findTraps()
{
	for (int i = 0; i < traps.size(); i++)
	{
		traps.at(i).setFound();
	}
}

int Room::getTrapDamage()
{
	int returnVal = 0;
	for (int i = 0; i < traps.size(); i++)
	{
		if (!traps.at(i).getFound())
			returnVal += traps.at(i).getDamage();
	}
	return returnVal;
}
Room * Room::moveTo(RoomDirection dir)
{
	if (hasConnection(dir))
	{
		connected[dir]->setVisiting(true);
		connected[dir]->setVisited();
		this->setVisiting(false);
		return connected[dir];
	}
	else
	{				
		return this;
	}

}

string Room::getDescription(){
	string returnVal = "";
	if (exit)
		returnVal += "THIS IS THE EXIT!!  \n";
	returnVal += "the " + appearance + " " + description + " has a very " + size + " size, there is a " + lighting + " above a " + content;
	return returnVal;
}

void Room::setRoom(RoomDirection dir, Room* r){
	connected[dir] = r;
	r->setFromOpposite(getOpposite(dir), this);
}



void Room::setFromOpposite(RoomDirection dir, Room* r){
	connected[dir] = r;

}

void Room::showDoors(){
	std::system("cls");
	std::cout << "VERBINDINGEN" << std::endl << "_________________________________________" << std::endl;
	if (connected[RoomDirection::NORTH] != nullptr)
		std::cout << "NORTH " << connected[RoomDirection::NORTH]->getDescription() << endl;
	if (connected[RoomDirection::EAST] != nullptr)
		std::cout << "EAST " << connected[RoomDirection::EAST]->getDescription() << endl;
	if (connected[RoomDirection::SOUTH] != nullptr)
		std::cout << "SOUTH " << connected[RoomDirection::SOUTH]->getDescription() << endl;
	if (connected[RoomDirection::WEST] != nullptr)
		std::cout << "WEST " << connected[RoomDirection::WEST]->getDescription() << endl;

}

RoomDirection Room::getOpposite(RoomDirection r){
	switch (r){
	case RoomDirection::NORTH:
		return RoomDirection::SOUTH;
		break;
	case RoomDirection::EAST:
		return RoomDirection::WEST;
		break;
	case RoomDirection::SOUTH:
		return RoomDirection::NORTH;
		break;
	default:
		return RoomDirection::EAST;
	}
}

bool Room::hasConnection(RoomDirection direction){
	if (this->connected[direction] != nullptr)
		return true;
	return false;
}

void Room::removeConnection(RoomDirection direction){
	if (hasConnection(direction))
	{
		Room * opposite = this->connected[direction];
		RoomDirection oppositeDirection = getOpposite(direction);
		if (opposite->hasConnection(oppositeDirection))
			opposite->setFromOpposite(oppositeDirection, nullptr);
		

	}
	this->connected[direction] = nullptr;
		
}

void Room::addEnemy(int lvl)
{
	enemys.push_back(Enemy(lvl));
}


void Room::showEnemys(){
	std::cout << enemys.size() << " ENEMYS" << std::endl;
	for (size_t i = 0; i < enemys.size(); i++)
	{
		std::cout << enemys.at(i).getDescription() << std::endl;
	}
	std::cout <<std::endl;
}


void Room::fight()
{
	int xp;
	if (enemys.size() < 1)
	{
		std::cout << "No enemys in the room." << std::endl;
	}
	else if (!checkEnemysAlive())
	{
		std::cout << "All enemys killed" << std::endl;
	}
	else
	{
		bool fighting = true;
		while (fighting){
			char input[100];
			cout << "Pres a key : ";
			cin.getline(input, sizeof(input));
			switch (input[0]){
			case 'f':
				for (size_t i = 0; i < enemys.size(); i++)
				{
					if (enemys.at(i).isAlive())
						enemys.at(i).attackMe(4);
				}
				break;
			case 'r':
				fighting = false;
				break;
			default:
				std::system("cls");
				std::cout << "unknown key" << std::endl;
				break;

			}
		}
	}
}

std::vector<Enemy*> Room::getEnemysAlive()
{
	std::vector<Enemy*> temp;
	for (int i = 0; i < enemys.size(); i++)
	{
		if (enemys.at(i).isAlive())
		{
			temp.push_back(&enemys.at(i));
		}
	}
	return temp;
}

bool Room::checkEnemysAlive()
{
	bool killedAll = true;
	for (size_t i = 0; i < enemys.size(); i++)
	{
		if (enemys.at(i).isAlive())
			return true;
	}
	return false;
}



bool Room::isExit()
{
	return exit;
}

void Room::setExit()
{
	exit = true;
}

int Room::getScore()
{
	int returnValue = 0;
	for (int i = 0; i < enemys.size(); i++)
	{ 
		returnValue += 20;
	}
	return returnValue;
}

std::vector<Equipment> Room::pickItems(int awareness)
{
	vector<Equipment> temp;
	if (!checkEnemysAlive())
	{
		for (int i = 0; i < equipment.size(); i++)
		{
			if (equipment.at(i).getAwarenes() <= awareness)
				temp.push_back(equipment.at(i));
		}
		for (int i = 0; i < equipment.size(); i++)
		{
			if (equipment.at(i).getAwarenes() <= awareness)
				equipment.erase(equipment.begin() + i);
		}
		
		if (temp.size() == 0)
			std::cout << "No equipment found" << std::endl;
		else
			std::cout << "You found some equipment!" << std::endl;
		return temp;
	}
	else
	{
		std::cout << "Kill all enemys first." << std::endl;
		return temp;
	}
}

void Room::showEquipment()
{
	std::cout << "Equipment found in this room." << std::endl;
	for (int i = 0; i < equipment.size(); i++)
	{
		if (equipment.at(i).isFound())
			std::cout << equipment.at(i).getName() << std::endl << std::endl;
	}
	std::cout << std::endl << "Equipment not found in this room." << std::endl;
	for (int i = 0; i < equipment.size(); i++)
	{
		if (!equipment.at(i).isFound())
			std::cout << equipment.at(i).getName() << std::endl;
	}
	std::cout << std::endl << std::endl;

}

void Room::findEquipment(int awareness)
{
	for (int i = 0; i < equipment.size(); i++)
	{
		if (equipment.at(i).getAwarenes() <= awareness)
			equipment.at(i).setFound();
	}
}