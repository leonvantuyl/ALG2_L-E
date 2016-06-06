#include "stdafx.h"
#include "Room.h"
#include <iostream>
#include "Boss.h"
#include "RandomGenerator.h"

Room::Room(int room)
{
	number = room;
	Boss endBoss = Boss(10);
	RandomGenerator rg;
	size = RoomSize[rg.getRandom(0, 2)];
	content = RoomContent[rg.getRandom(0, 3)];
	description = RoomDescription[rg.getRandom(0, 3)];
	appearance = RoomAppearance[rg.getRandom(0, 3)];
	lighting = RoomLighting[rg.getRandom(0, 3)];
	color = RoomColor[rg.getRandom(0, 3)];
	connected[RoomDirection::NORTH] = nullptr;
	connected[RoomDirection::EAST] = nullptr;
	connected[RoomDirection::SOUTH] = nullptr;
	connected[RoomDirection::WEST] = nullptr;
	visited = false;
	visiting = false;
	exit = false;
	equipment.push_back(Equipment());
	if (rg.getRandom(0, 100) < 20)
		traps.push_back(Trap());
}


Room::~Room()
{
}

string Room::getDangerValue()
{
	int value = 0;
	for (EnemyBase enemy : enemys) {
		value += enemy.health;
	}
	for (Trap trap : traps) {
		value += trap.getDamage();
	}
	return to_string(value);
}

string Room::getSymbolCheat() {
	if (visiting)
		return "P";
	else if (!exit)
		return getDangerValue();
	else if (exit)
		return "S";
	else
		return "X";
}

string Room::getSymbol() {
	if (visiting)
		return "P";
	else if (visited & !exit)
		return "X";
	else if (visited & exit)
		return "S";
	else
		return ".";

}

bool Room::getVisited()
{
	return visited;
}

void Room::killAllEnemys()
{
	for (size_t i = 0; i < enemys.size(); i++)
	{
		enemys.at(i).killEnemy();
	}
}

void Room::setVisiting(bool isVisiting)
{
	visiting = isVisiting;
}

void Room::setVisited()
{
	visited = true;
}

void Room::findTraps(int lvl)
{
	for (size_t i = 0; i < traps.size(); i++)
	{
		if (lvl > traps.at(i).getMinimalLevel())
		{
			traps.at(i).setFound();
			cout << "Trap found" << endl;
		}
		else
		{

			cout << "Level to low to find the trap " << traps.at(i).getMinimalLevel() << "." << endl;
		}

	}
}

int Room::getTrapDamage()
{
	int returnVal = 0;
	for (size_t i = 0; i < traps.size(); i++)
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

Room* Room::getRoom(RoomDirection direction)
{
	return connected.at(direction);
}

string Room::getDescription() {
	string returnVal = "";
	if (exit)
		returnVal += "THIS IS THE EXIT!!  \n";
	returnVal += "the " + color + " " + appearance + " " + description + " has a very " + size + " size, there is a " + lighting + " above a " + content;
	return returnVal;
}

void Room::setRoom(RoomDirection dir, Room* r) {
	connected[dir] = r;
	r->setFromOpposite(getOpposite(dir), this);
}

void Room::setFromOpposite(RoomDirection dir, Room* r) {
	connected[dir] = r;

}

void Room::showDoors() {
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

//Convert map to vector for easy use
vector<Room*> Room::getConnectedRooms()
{
	vector<Room *> connectedRooms;
	for (auto const &ent : connected) {
		if (ent.second != nullptr)
			connectedRooms.push_back(ent.second);
	}
	return connectedRooms;
}

RoomDirection Room::getOpposite(RoomDirection r) {
	switch (r) {
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

RoomDirection Room::findCollapseRoomDirection(Room* room)
{
	typedef std::map<RoomDirection, Room*>::iterator it_type;
	for (it_type iterator = connected.begin(); iterator != connected.end(); iterator++) {
		// iterator->first = key
		// iterator->second = value

		if (iterator->second == room)
		{
			//brokenConnections.at(iterator->first) = true;
			return iterator->first;
		}
	}
	return RoomDirection::ERROR;
}

bool Room::hasConnection(RoomDirection direction) {
	if (this->connected[direction] != nullptr)
		return true;
	return false;
}

void Room::removeConnection(RoomDirection direction) {
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


void Room::showEnemys() {
	std::cout << enemys.size() << " ENEMYS" << std::endl;
	for (size_t i = 0; i < enemys.size(); i++)
	{
		std::cout << enemys.at(i).getDescription() << std::endl;
	}
	std::cout << std::endl;
}


void Room::fight()
{
	//TODO ? deze variable werd niet gebruikt. int xp;
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
		while (fighting) {
			char input[100];
			cout << "Press a key : ";
			cin.getline(input, sizeof(input));
			switch (input[0]) {
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

std::vector<EnemyBase*> Room::getEnemysAlive()
{
	std::vector<EnemyBase*> temp;
	for (size_t i = 0; i < enemys.size(); i++)
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

void Room::setExit(int lvl)
{
	exit = true;
	enemys.push_back(Boss(lvl));
}

int Room::getScore()
{
	int returnValue = 0;
	for (size_t i = 0; i < enemys.size(); i++)
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
		for (size_t i = 0; i < equipment.size(); i++)
		{
			if (equipment.at(i).getAwarenes() <= awareness)
				temp.push_back(equipment.at(i));
		}
		for (size_t i = 0; i < equipment.size(); i++)
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
	std::cout << "Equipment that you did find in this room:" << std::endl;
	for (size_t i = 0; i < equipment.size(); i++)
	{
		if (equipment.at(i).isFound())
			std::cout << equipment.at(i).getName() << std::endl << std::endl;
	}
	std::cout << std::endl << "Equipment that is hidden from eyes:" << std::endl;
	for (size_t i = 0; i < equipment.size(); i++)
	{
		if (!equipment.at(i).isFound())
			std::cout << equipment.at(i).getName() << std::endl;
	}
	std::cout << std::endl << std::endl;

}

void Room::findEquipment(int awareness)
{
	for (size_t i = 0; i < equipment.size(); i++)
	{
		if (equipment.at(i).getAwarenes() <= awareness)
			equipment.at(i).setFound();
	}
}

void Room::print(int number) {
	if (!isExit())
		std::cout << to_string(number) << " : " << currentValueDijkstra << std::endl;
	else
		std::cout << to_string(number) << " : exit" << std::endl;
}