#pragma once
#include "Room.h"
#include "Edge.h"
#include <vector>
#include "Player.h"
	class Level
	{
	private:
		Room * currentPosition;
		Player * p1;
		int ID;
		int levelWidth;
		int levelHeight;
		bool finished;
		std::vector < std::vector<Room> >  rooms;
		std::vector < Edge >  edges;
		bool compareRoomWithVector( Room*, vector<Room *> );
		bool compareEdgeWithVector( Edge current, vector<Edge> rooms );

	public:
		Level(int id, Player * pl);
		~Level();
		void resetDijkstra();
		void printLevel(bool);		
		void move(RoomDirection);
		void showDescription();
		void findBombs();
		bool isFinished();
		void moveUp();
		void setFinished();
		void pickItems();
		void fight();
		void printFightHelp();
		void setInitialPosition();
		void findEquipment(int);
		int getTrapDamage();

		void grenade();
		int magicTalisman();
		void compass(bool cheat);
	};

