#pragma once
#include "creature.h"

class NPC : public Creature {
public:
	NPC(const char* name, const char* description, Room* location);	
};
