#include "stdafx.h"
#include "Game.h"

#include "Level.h"
#include <iostream>

Game::Game()
{
	levels = new std::vector<Level*>();
	for (int i = 0; i < 10; i++)
	{
		
		levels->push_back(new Level(i, &p1));
	}
	currentLevelID = 0;
	currentLevel = levels->at(0);
	currentLevel->printLevel();
}


Game::~Game()
{
	for (size_t i = 1; i < levels->size(); i++)
	{
		delete levels->at(i);
		levels->at(i) = nullptr;
	}
	delete levels;
	levels = nullptr;
}

void Game::start(){
	bool running = true;
	std::cout << p1.getName() << ", welcome game started." << std::endl;
	cout << "Do you want to play a game? : " ;
	char startme[100];
	cin.getline( startme, sizeof( startme ) );
	printHelp();
	while (running){
		if (checkForFinished())
		{
			running = false; 
			return;
		}
			
		char input[100];
		cout << "Press a key : ";
		cin.getline(input, sizeof(input));
		switch (input[0]){
		case 'w':
			currentLevel->move(RoomDirection::NORTH);
			currentLevel->showDescription();
			break;
		case 'a':
			currentLevel->move(RoomDirection::WEST);
			currentLevel->showDescription();
			break;
		case 'd':
			currentLevel->move(RoomDirection::EAST);
			currentLevel->showDescription();
			break;
		case 's':
			currentLevel->move(RoomDirection::SOUTH);
			currentLevel->showDescription();
			break;
		case 'u':
			currentLevel->moveUp();
			break;
		case 'p':
			//TODO klopt niet met beschrijving en wat doet deze methode. 
			currentLevel->pickItems();
			break;
		case 'z':
			p1.showStats();
			break;
		case 'r':
			p1.rest();
			break;
		case 'f':
			currentLevel->fight();
			break;
		case 'b':
			//TODO dit is een cheatcode
			currentLevel->setFinished();
			break;
		case 'e':
			currentLevel->showDescription();
			break;
		case 'h':
			printHelp();
			break;
		case 'm':
			currentLevel->printLevel();
			break;
		case  'm?':
			//TODO cheat mode
			//currentLevel->printLevel(true);
		case 'q':
			running = false;
		case 't':
			std::cout << "You look into your magic talisman." << std::endl << "The magic talisman reveals a number through it's crystal. " << std::endl;
			std::cout << currentLevel->magicTalisman() << " Room(s) remaining to the stairs." << std::endl;
			break;
		case 'g':
			currentLevel->grenade();
			currentLevel->printLevel();
			std::cout << "You throw the grenade at the enemy, it explodes and the whole dungeon shakes!" << std::endl << "The enemys are no more than ashes, but it feels like the dungeon has changed." << std::endl;
			break;
		default:
			std::system("cls");
			std::cout << "unknown key" << std::endl;
			printHelp();
			break;
		}
		/*if (!p1.isAlive())
		{
			system( "cls" );
			cout << "You are dead sucker" << endl;
		}*/
	}
}

void Game::printHelp(){

	std::system("cls");
	std::cout << "HELP" << std::endl << "_________________________________________" << std::endl;
	std::cout << " M : show map." << std::endl;
	std::cout << " E : show current room." << std::endl;
	std::cout << " P : pick items in room." << std::endl;
	std::cout << " W : move north." << std::endl;
	std::cout << " D : move east." << std::endl;
	std::cout << " S : move south." << std::endl;
	std::cout << " A : move west." << std::endl;
	std::cout << " P : pick up item" << std::endl;
	std::cout << " R : Rest." << std::endl;
	std::cout << " U : move up the stairs." << std::endl;
	std::cout << " Z : Show stats." << std::endl;	
	std::cout << " H : help." << std::endl;
	std::cout << " Q : quit." << std::endl;
	std::cout << "HELP" << std::endl << "_________________________________________" << std::endl;
}

bool Game::checkForFinished()
{
	if (currentLevel->isFinished())
	{
		if (currentLevelID < levels->size() - 1)
		{
			currentLevelID++;
			currentLevel = levels->at(currentLevelID);
			currentLevel->printLevel();

			std::cout << "Level completed" << std::endl;
		}
		else
		{

			std::system("cls");
			std::cout << "You completed the game!!" << std::endl;
			char endMe;
			std::cin >> endMe;
			return true;
		}
	}
	return false;

		
}

