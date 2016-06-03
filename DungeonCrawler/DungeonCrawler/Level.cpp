#include "stdafx.h"
#include "Level.h"

#include <iostream>
#include <random>
#include <queue>
#include "RandomGenerator.h"
Level::Level(int id, Player * pl)
{
	p1 = pl;
	finished = false;
	ID = id;

	RandomGenerator rg;
	levelWidth = rg.getRandom(4, 7);
	levelHeight = rg.getRandom(4, 7);;

	int number = 0;
	//Rooms inladen
	for (int y = 0; y < levelHeight; y++)
	{
		std::vector<Room> temp;
		for (int x = 0; x < levelWidth; x++)
		{
			number++;
			temp.push_back(Room(number));
		}
		rooms.push_back(temp);
	}

	//Verbindingen instellen
	for (int y = 0; y < levelHeight; y++)
	{

		string bottomConnection;
		for (int x = 0; x < levelWidth; x++)
		{
			Room *temp;

			temp = &rooms[y][x];

			// Vijand toevoegen op basis van random getal, als level hoger wordt , kans groter

			if (rg.getRandom(0, 5) < (1 + ID))
				temp->addEnemy(ID);

			//--

			if (x < levelWidth - 1)
			{
				edges.push_back(Edge(temp, &rooms[y][x + 1], RoomDirection::EAST));
				temp->setRoom(RoomDirection::EAST, &rooms[y][x + 1]);
			}
			if (y < levelHeight - 1) {
				edges.push_back(Edge(temp, &rooms[y + 1][x], RoomDirection::SOUTH));
				temp->setRoom(RoomDirection::SOUTH, &rooms[y + 1][x]);
			}

		}
	}

	setInitialPosition();

	/*Room *temp = &rooms[0][0];
	temp->removeConnection(RoomDirection::NORTH);
	temp = &rooms[3][3];
	temp->removeConnection(RoomDirection::EAST);
	temp = &rooms[1][1];
	temp->removeConnection(RoomDirection::WEST);*/
}


Level::~Level()
{
}



void Level::move(RoomDirection dir)
{
	Room * temp = currentPosition;
	int damage = currentPosition->getTrapDamage();
	currentPosition = currentPosition->moveTo(dir);

	printLevel(false);
	if (temp == currentPosition)
	{
		std::cout << "Not able to move in that direction" << std::endl;
	}
	else
	{
		if (damage > 0)
		{
			std::cout << "There were hidden traps in the last room Damage " << damage << std::endl;
			p1->takeDamage(damage);
		}
	}


}

void Level::showDescription()
{
	printLevel(false);
	findBombs();
	std::cout << currentPosition->getDescription() << std::endl;
	//TODO enemy hoor je gelijk tegen te komen als je een kamer binnen komt.
	currentPosition->showEnemys();
	currentPosition->findEquipment(p1->getAwarenes());
	currentPosition->showEquipment();
	p1->addXP(2);

}

bool Level::isFinished()
{
	return finished;
}

void Level::findBombs()
{
	currentPosition->findTraps(p1->getAwarenes());

}

int Level::getTrapDamage()
{
	return currentPosition->getTrapDamage();
}

void Level::grenade()
{
	//Setup 
	vector<Edge> mst;
	vector<Room*> visitedRooms;
	queue<Room*> roomQueue;
	Room* current = currentPosition;
	int destroyedConnections = 0;

	//First room setup
	visitedRooms.push_back(currentPosition);
	roomQueue.push(currentPosition);

	// Continue while not enough connections destroyed yet 
	while (destroyedConnections < 10)
	{
		if (!roomQueue.empty())
		{
			current = roomQueue.front();
			roomQueue.pop();
		}
		else
		{
			destroyedConnections = 11;
		}

		//Search current connected rooms
		vector<Room*> currentConnected = current->getConnectedRooms();
		for (Room* room : currentConnected)
		{
			if (!compareRoomWithVector(room, visitedRooms))
			{
				mst.push_back(Edge(current, room));
				visitedRooms.push_back(room);
				roomQueue.push(room);
			}
			else if (!compareEdgeWithVector(Edge(current, room), mst))
			{
				RoomDirection direction = current->findCollapseRoomDirection(room);
				current->removeConnection(direction);
				destroyedConnections++;
			}
		}

	}


	//TODO Blaast 10-15 corridors op. 
	//visited corridors worden ook als gesloopt getoond.
	//Gebruikt de minimum spanning tree
}

bool Level::compareEdgeWithVector(Edge current, vector<Edge> edges)
{
	bool found = false;
	for (Edge edge : edges)
	{
		if (edge.r1 == current.r1 || edge.r1 == current.r2)
		{
			if (edge.r2 == current.r1 || edge.r2 == current.r2)
			{
				found = true;
			}
		}
	}
	return found;
}

//TODO test
//The magicTalisman uses a breadth first search algorithm to find the stairs from your current position within the least amount of steps.
//Returns the minimal amount of steps needed.
int Level::magicTalisman()
{
	//initial setup
	vector<Room*> visitedRooms;
	queue<Room*> roomQueue;
	Room * current = nullptr;
	bool found = false;
	int requiredSteps = 0;

	//first room setup
	visitedRooms.push_back(currentPosition);
	roomQueue.push(currentPosition);
	currentPosition->requiredSteps = 0;

	//Check if the first room is the exit. if true then while loop should be skiped.
	if (currentPosition->isExit()) {
		found = true;
	}

	//Search until exit room is found
	while (!found) {
		//Check next room
		if (!roomQueue.empty()) {
			current = roomQueue.front();
			roomQueue.pop();
			requiredSteps = current->requiredSteps + 1;
		}
		else {
			//error
			requiredSteps = -1;
			found = true;
		}

		//Search current connected rooms
		vector<Room*> currentConnected = current->getConnectedRooms();
		for (Room * room : currentConnected) {
			if (!compareRoomWithVector(room, visitedRooms)) {
				room->requiredSteps = requiredSteps;
				visitedRooms.push_back(room);
				roomQueue.push(room);
				if (room->isExit()) {
					found = true;
				}
			}
		}
	}
	return requiredSteps;
}

//returns true if the current room is contained within the vector of rooms
bool Level::compareRoomWithVector(Room * current, vector<Room*> rooms) {
	for (Room* room : rooms) {
		if (current == room) {
			return true;
		}
	}
	return false;
}

void Level::compass()
{
	//Reset so distances are 0(current) and -1(others)
	resetDijkstra();

	//initial setup
	vector<Room*> spt;
	vector<Room*> sptSet;
	Room * current = currentPosition;
	sptSet.push_back(current);
	bool found = false;

	//Check if this is the exit.
	if (current->isExit()) {
		found = true;
		spt.push_back(current);
		std::cout << "already at the exit" << std::endl;
	}

	//loop
	while (!found) {
		//Pick next lowest value;	
		current = sptSet.front();
		for (Room * room : sptSet) {
			if (!room->isExit() && !found) {
				if (room->currentValueDijkstra < current->currentValueDijkstra) {
					current = room;
				}
			}
			else if(!found) {
				current = room;
				found = true;				
			}
		}

		if (!found) {
			vector<Room*> currentConnected = current->getConnectedRooms();
			for (Room * room : currentConnected) {
				//Check if the room already is in the shortest path.
				if (!compareRoomWithVector(room, spt)) {
					//check if the room is already in the set of not than add
					if (!compareRoomWithVector(room, sptSet)) {
						sptSet.push_back(room);
						room->currentValueDijkstra = current->currentValueDijkstra + stoi(room->getDangerValue());
					}
					else {
						//calculate new value and replace if needed
						int calculatedValue = current->currentValueDijkstra + stoi(room->getDangerValue());
						if (calculatedValue < room->currentValueDijkstra)
							room->currentValueDijkstra = calculatedValue;
					}
				}
			}
			//remove current from sptset
			sptSet.erase(std::remove(sptSet.begin(), sptSet.end(), current), sptSet.end());

		}
		//push back to shortest path
		spt.push_back(current);
	}

	int i = spt.size() - 1;
	current = spt.back();
	while (current != spt.front()) {
		Room * next = spt[i - 1];
		RoomDirection dir = current->findCollapseRoomDirection(next);
		if (dir != RoomDirection::ERROR) {
			switch (dir) {
			case RoomDirection::EAST:
				std::cout << "East" << std::endl;;
				break;
			case RoomDirection::WEST:
				std::cout << "West" << std::endl;;
				break;
			case RoomDirection::NORTH:
				std::cout << "North" << std::endl;;
				break;
			case RoomDirection::SOUTH:
				std::cout << "South" << std::endl;;
				break;
			}
			i--;
			current = spt[i];
		}
		else {
			i--;
		}
		
	}


	int number = 0;
	for (Room * room : spt) {
		room->print(number);
		number++;
	}
}

void Level::resetDijkstra() {
	//initial setup
	vector<Room*> visitedRooms;
	queue<Room*> roomQueue;
	Room * current = currentPosition;

	//first room setup
	visitedRooms.push_back(current);
	roomQueue.push(current);
	current->currentValueDijkstra = 0;

	while (!roomQueue.empty()) {
		current = roomQueue.front();
		roomQueue.pop();

		//Search current connected rooms
		vector<Room*> currentConnected = current->getConnectedRooms();
		for (Room * room : currentConnected) {
			if (!compareRoomWithVector(room, visitedRooms)) {
				visitedRooms.push_back(room);
				roomQueue.push(room);
				room->currentValueDijkstra = std::numeric_limits<int>::max();;
			}
		}
	}
}

void Level::pickItems()
{
	vector<Equipment> temp = currentPosition->pickItems(p1->getAwarenes());
	for (size_t i = 0; i < temp.size(); i++)
	{
		p1->addEquipment(temp.at(i));
	}
}

void Level::printLevel(bool cheatMode) {
	std::system("cls");
	vector< vector<int> >::iterator row;
	vector<int>::iterator col;
	std::cout << std::endl;
	for (int y = 0; y < levelHeight; y++)
	{
		std::cout << " ";
		string bottomConnection;
		for (int x = 0; x < levelWidth; x++)
		{
			Room *temp;
			temp = &rooms[y][x];
			string symbol = "";
			if (cheatMode)
				symbol = temp->getSymbolCheat();
			else
				symbol = temp->getSymbol();
			std::cout << symbol;

			if (!temp->getVisited())
			{
				Room *tempEast = temp->getRoom(RoomDirection::EAST);
				if (tempEast == nullptr)
					std::cout << "   ";
				else if (x != levelWidth - 1)
					if (!tempEast->getVisited() && !cheatMode)
						std::cout << "???";
					else
						std::cout << "---";

				Room *tempSouth = temp->getRoom(RoomDirection::SOUTH);
				if (tempSouth == nullptr)
					bottomConnection += "    ";
				else if (y != levelHeight - 1)
					if (!tempSouth->getVisited() && !cheatMode)
						bottomConnection += "?   ";
					else
						bottomConnection += "|   ";

			}
			else
			{
				if (x != levelWidth - 1)
				{
					if (!temp->hasConnection(RoomDirection::EAST))
						std::cout << "   ";
					else
						std::cout << "---";
				}
				if (y != levelHeight - 1) {
					if (!temp->hasConnection(RoomDirection::SOUTH))
						bottomConnection += "    ";
					else
						bottomConnection += "|   ";
				}
			}



		}
		std::cout << " " << endl;
		std::cout << " " << bottomConnection << endl << " " << bottomConnection << endl;
	}
}

void Level::moveUp()
{
	if (currentPosition->isExit())
		finished = true;
	else
		std::cout << "Current room has no stairs" << std::endl;
}

void Level::setFinished()
{
	finished = true;
}

void Level::findEquipment(int awareness)
{
	currentPosition->findEquipment(p1->getAwarenes());
}

void Level::setInitialPosition()
{
	random_device dev;

	//Start positie van de speler bepalen
	uniform_int_distribution<int> distExitPosX{ 0, levelWidth - 1 };
	uniform_int_distribution<int> distExitPosY{ 0, levelHeight - 1 };
	uniform_int_distribution<int> distStartPosX{ 0, levelWidth - 1 };
	uniform_int_distribution<int> distStartPosY{ 0, levelHeight - 1 };

	int startPosX = distStartPosX(dev);
	int startPosy = distStartPosY(dev);
	int exitPosX = distExitPosX(dev);
	int exitPosY = distExitPosY(dev);

	currentPosition = &rooms[startPosy][startPosX];
	currentPosition->setVisiting(true);
	currentPosition->setVisited();

	Room *tempExit = &rooms[exitPosY][exitPosX];
	tempExit->setExit(ID);

}

void Level::fight()
{
	int xp = 0;
	if (!currentPosition->checkEnemysAlive())
	{
		std::cout << "No alive enemys in the room." << std::endl;
	}
	else
	{
		RandomGenerator rg;
		std::vector<EnemyBase*> temp = currentPosition->getEnemysAlive();
		for (size_t i = 0; i < temp.size(); i++)
		{
			xp += temp.at(i)->getXP();
		}

		printFightHelp();
		std::cout << "Trouble has arrived" << std::endl;
		bool fighting = true;
		while (fighting) {
			char input[100];
			cout << "Pres a key : ";
			cin.getline(input, sizeof(input));
			switch (input[0]) {
			case 'a':
			{
				for (size_t i = 0; i < temp.size(); i++)
				{
					if (temp.at(i)->isAlive())
						temp.at(i)->attackMe(p1->doAttack());
				}
				currentPosition->showEnemys();
			}

			break;
			case 'r':
				fighting = false;
				break;
			case 'i':
			{
				bool picked = false;
				while (!picked)
				{
					char input[100];
					cout << "Pick your item (q to close inventory) : " << endl;
					p1->printInventory();
					cin.getline(input, sizeof(input));
					if ('0' <= input[0] && input[0] <= '9')
					{
						Equipment * tempWep = p1->getEquipment(input[0] - 48);
						if (tempWep != nullptr)
						{
							for (size_t i = 0; i < temp.size(); i++)
							{
								if (temp.at(i)->isAlive())
									temp.at(i)->attackMe(tempWep->getHitpoints());
							}
							picked = true;
						}



					}
					else if (input[0] == 'q')
					{
						std::cout << "What? You don't want to pick something? Alright." << endl;
						picked = true;
					}
					else
					{
						std::cout << "Insert a number from your inverntory." << endl;
					}
				}
			}
			break;
			default:
				std::system("cls");
				std::cout << "unknown key" << std::endl;
				printFightHelp();
				break;
			}


			if (!currentPosition->checkEnemysAlive())
			{
				fighting = false;
				printLevel(false);
				std::cout << "You killed all enemys, fight ended!" << endl;
				p1->addXP(xp);
			}
			else
			{
				std::cout << "ENEMYS ATTACKING!!" << std::endl;
				for (size_t i = 0; i < temp.size(); i++)
				{
					if (temp.at(i)->isAlive())
					{
						int rndmAttack = rg.getRandom(0, 1000);
						if (rndmAttack > p1->getDefence())
						{
							int dmg = temp.at(i)->attack();
							std::cout << temp.at(i)->getDescription() << " did " << dmg << " damage." << std::endl;
							p1->takeDamage(dmg);
						}
						else
						{
							std::cout << "Thanks to your great defence he missed" << std::endl;
						}


					}

				}
			}
		}
	}
}

void Level::printFightHelp()
{
	std::system("cls");
	std::cout << "FIGHTING HELP" << std::endl << "_________________________________________" << std::endl;
	std::cout << " A : Attack enemy." << std::endl;
	std::cout << " R : Run away" << std::endl;
	std::cout << " I : chose from inventory" << std::endl;
	std::cout << "FIGHTING HELP" << std::endl << "_________________________________________" << std::endl;
}