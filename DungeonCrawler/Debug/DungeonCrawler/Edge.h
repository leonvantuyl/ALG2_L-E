#pragma once
#include "Room.h"

class Edge
{
	Room * r1;
	Room * r2;
	bool visited;
	RoomDirection direction;
public:
	

	Edge(Room *, Room *, RoomDirection);
	~Edge();
};

