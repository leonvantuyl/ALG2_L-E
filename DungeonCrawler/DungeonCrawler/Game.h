#pragma once
#include "Level.h"
#include "Enemy.h"
#include <iostream>
#include <vector>
#include "Level.h"
#include "Player.h"
class Game
{
private:
	Level* currentLevel;
	int levelID = 0;
	int currentLevelID;
	Player p1 = Player("Dave");
	std::vector<Level*> *levels;
public:
	Game();
	~Game();

	
	void start();
	void printHelp();
	bool checkForFinished();
};

