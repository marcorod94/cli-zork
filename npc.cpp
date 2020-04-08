#include "npc.h"

NPC::NPC(const char * name, const char * description, Room * location) :
	Creature(name, description, location) {
	type = EntityType::ENEMY;
}