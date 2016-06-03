#pragma once
#include <map>
#include <string>
#include "Enemy.h"
#include "Equipment.h"
#include <vector>
#include "Trap.h"
using namespace std;
enum class RoomDirection { NORTH, EAST, SOUTH, WEST, ERROR };


const string  RoomSize[]{ "small", "average", "big" };
const string  RoomContent[]{ "table", "closet", "bed", "harp" };
const string  RoomDescription[]{ "dungeon", "kitchen", "bedroom", "bathroom" };
const string  RoomAppearance[]{ "cleaned", "dirty", "scary", "funny" };
const string  RoomLighting[]{ "fluorescenttube", "candle", "torch", "fireplace" };
const string  RoomColor[]
{
	"blue", "red", "orange", "black"
};

class Room
{
private:
	map<RoomDirection, Room*> connected; //Map containing all possible corridors
	string size;
	string content;
	string description;
	string appearance;
	string lighting;
	string color;
	bool visited;
	bool visiting;
	bool exit;
	std::vector<EnemyBase> enemys;
	std::vector<Trap> traps;
	std::vector<Equipment> equipment;
	int number;
public:
	Room(int);
	~Room();
	string getDangerValue();
	int requiredSteps;
	int currentValueDijkstra;

	void setRoom(RoomDirection, Room*);
	void setFromOpposite(RoomDirection, Room*);
	void showDoors();
	vector<Room*> getConnectedRooms();

	std::vector<Equipment> pickItems(int);
	void addEnemy(int lvl);
	void showEnemys();
	void setVisited();
	void setVisiting(bool);
	bool hasConnection(RoomDirection);
	bool hasTrap();
	void removeConnection(RoomDirection);
	void findTraps(int);
	void fight();
	void showEquipment();
	void findEquipment(int awareness);
	bool checkEnemysAlive();
	void setExit(int lvl);
	bool isExit();
	int getScore();
	int getTrapDamage();
	void print(int number);
	std::vector<EnemyBase*> getEnemysAlive();
	Room* moveTo(RoomDirection);
	Room* getRoom(RoomDirection);
	string getDescription();
	string getSymbol();
	string getSymbolCheat();
	bool getVisited();
	void killAllEnemys();
	RoomDirection getOpposite(RoomDirection);
	void printDirection(Room *);

	
	void collapseDirection(RoomDirection);
	RoomDirection findCollapseRoomDirection(Room*);

};


