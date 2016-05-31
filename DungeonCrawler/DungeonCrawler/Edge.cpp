#include "stdafx.h"
#include "Edge.h"


Edge::Edge(Room * room1, Room * room2, RoomDirection dir)
{
	visited = false;
	r1 = room1;
	r2 = room2;
	direction = dir;
}


Edge::~Edge()
{
}
