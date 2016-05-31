#pragma once
#include <map>
#include <string>
#include "Enemy.h"
#include "Equipment.h"
#include <vector>
#include "Trap.h"
using namespace std;
enum class RoomDirection { NORTH, EAST, SOUTH, WEST };


const string  RoomSize[]{ "small", "average", "big" };
const string  RoomContent[]{ "table", "closet", "bed", "harp" };
const string  RoomDescription[]{ "dungeon", "kitchen", "bedroom", "bathroom" };
const string  RoomAppearance[]{ "cleaned", "dirty", "scary", "funny" };
const string  RoomLighting[]{ "fluorescenttube", "candle", "torch", "fireplace" };

class Room
{
private:
	map<RoomDirection, Room*> connected;
	string size;
	string content;
	string description;
	string appearance;
	string lighting;
	bool visited;
	bool visiting;
	bool exit;
	std::vector<Enemy> enemys;
	std::vector<Trap> traps;
	std::vector<Equipment> equipment;

public:
	Room();
	~Room();

	void setRoom(RoomDirection, Room*);
	void setFromOpposite(RoomDirection, Room*);
	void showDoors();


	std::vector<Equipment> pickItems(int);
	void addEnemy(int lvl);
	void showEnemys();
	void setVisited();
	void setVisiting(bool);
	bool hasConnection(RoomDirection);
	bool hasTrap();
	void removeConnection(RoomDirection);
	void findTraps();
	void fight();
	void showEquipment();
	void findEquipment(int awareness);
	bool checkEnemysAlive();
	void setExit();
	bool isExit();
	int getScore();
	int getTrapDamage();
	std::vector<Enemy*> getEnemysAlive();
	Room * moveTo(RoomDirection);
	Room* getRoom(RoomDirection);
	string getDescription();
	string getSymbol();
	string getSymbolCheat();
	bool getVisited();
	RoomDirection getOpposite(RoomDirection);
};


