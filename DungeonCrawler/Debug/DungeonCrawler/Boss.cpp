#include "stdafx.h"
#include "Boss.h"


Boss::Boss(int lvl)
{
	level = lvl;
	name = "Boss'" + lvl;
	state = "Furious";
}


Boss::~Boss()
{
}
