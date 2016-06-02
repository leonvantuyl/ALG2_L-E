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

public:
	Room();
	~Room();
	int getDangerValue();
	int requiredSteps;

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
	std::vector<EnemyBase*> getEnemysAlive();
	Room* moveTo(RoomDirection);
	Room* getRoom(RoomDirection);
	string getDescription();
	string getSymbol();
	string getSymbolCheat();
	bool getVisited();
	RoomDirection getOpposite(RoomDirection);

	//TODO invullen
	void collapseDirection(RoomDirection);
	RoomDirection findCollapseRoomDirection(Room*);

};


