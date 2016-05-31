#include "stdafx.h"
#include "Level.h"

#include <iostream>
#include <random>
#include "RandomGenerator.h"
Level::Level(int id, Player * pl)
{
	p1 = pl;
	finished = false;
	ID = id;

	RandomGenerator rg;
	levelWidth = rg.getRandom(4,7);
	levelHeight = rg.getRandom(4, 7);;

		

	//Rooms inladen
	for (int y = 0; y < levelHeight; y++)
	{
		std::vector<Room> temp;
		for (int x = 0; x < levelWidth; x++)
		{
			temp.push_back(Room());
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
				temp->setRoom(RoomDirection::EAST, &rooms[y][x+1]);					
			}
			if (y < levelHeight - 1){
				edges.push_back(Edge(temp, &rooms[y + 1][x], RoomDirection::SOUTH));
				temp->setRoom(RoomDirection::SOUTH, &rooms[y + 1][x]);
			}
					
		}
	}

	setInitialPosition();

	Room *temp = &rooms[0][0];
	temp->removeConnection(RoomDirection::NORTH);
	temp = &rooms[3][3];
	temp->removeConnection(RoomDirection::EAST);
	temp = &rooms[1][1];
	temp->removeConnection(RoomDirection::WEST);
}


Level::~Level()
{
}



void Level::move(RoomDirection dir)
{	
	Room * temp = currentPosition;
	int damage = currentPosition->getTrapDamage();
	currentPosition = currentPosition->moveTo(dir);	
	
	
	printLevel();
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
	printLevel();
	findBombs();
	std::cout << currentPosition->getDescription() << std::endl;
	currentPosition->showEnemys();
	currentPosition->findEquipment( p1->getAwarenes() );
	currentPosition->showEquipment();
	p1->addXP( 2 );
	
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

void Level::pickItems()
{
	vector<Equipment> temp = currentPosition->pickItems(p1->getAwarenes());
	for (int i = 0; i < temp.size(); i++)
	{
		p1->addEquipment(temp.at(i));
	}
}

void Level::printLevel(){

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
			std::cout << temp->getSymbol();

			if (!temp->getVisited())
			{
				if (x != levelWidth -1)
					std::cout << "???";
				if (y != levelHeight -1)
					bottomConnection += "?   ";
			}
			else
			{
				if (x != levelWidth -1)
				{
					if (!temp->hasConnection(RoomDirection::EAST))
						std::cout << "   ";
					else
						std::cout << "---";
				}				
				if (y != levelHeight -1){

					if (temp->hasConnection(RoomDirection::SOUTH))
						bottomConnection += "|   ";
					else
						bottomConnection += "    ";
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
	uniform_int_distribution<int> distExitPosX{ 0, levelWidth -1 };
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
		while (fighting){
			char input[100];
			cout << "Pres a key : ";
			cin.getline(input, sizeof(input));
			switch (input[0]){
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
				while( !picked )
				{
					char input[100];
					cout << "Pick your item (q to close inventory) : " << endl;
					p1->printInventory();
					cin.getline( input, sizeof( input ) );
					if( '0' <= input[0] && input[0] <= '9' )
					{
						Equipment * tempWep = p1->getEquipment( input[0] - 48 );
						if( tempWep != nullptr )
						{
							for( size_t i = 0; i < temp.size(); i++ )
							{
								if( temp.at( i )->isAlive() )
									temp.at( i )->attackMe( tempWep->getHitpoints() );
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
				printLevel();
				std::cout << "You killed all enemys, fight ended!" << endl;
				p1->addXP(xp);
			}
			else
			{
				std::cout << "ENEMYS ATTACKING!!" << std::endl;
				for( size_t i = 0; i < temp.size(); i++ )
				{
					if( temp.at( i )->isAlive() )
					{
						int rndmAttack = rg.getRandom( 0, 1000 );
						if( rndmAttack > p1->getDefence() )
						{
							int dmg = temp.at( i )->attack();
							std::cout << temp.at( i )->getDescription() << " did " << dmg << " damage." << std::endl;
							p1->takeDamage( dmg );
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