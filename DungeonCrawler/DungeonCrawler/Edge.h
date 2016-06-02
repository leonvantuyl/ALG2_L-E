#pragma once
#include "Room.h"

class Edge
{
public:
	Room * r1;
	Room * r2;
	bool visited;
	RoomDirection direction;
	Edge(Room *, Room *, RoomDirection);
	Edge( Room*, Room* );
	~Edge();
};

