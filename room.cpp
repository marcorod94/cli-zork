#include "creature.h"
#include "exit.h"
#include "item.h"
#include "room.h"
#include <iostream>

Room::Room(const char* name, const char* description) :
	Entity(name, description, nullptr) {
}

void Room::Look() const {
	Entity::Look();
	for (Entity* entity : contains) {
		if (entity->type == EntityType::EXIT) {
			Exit* exit = (Exit*)entity;
			std::cout << "\nDirection (" << exit->GetNameFrom(this) << ") you see " << exit->GetDestinationFrom(this)->name;
		}
	}

	// List items --
	for (Entity* entity : contains) {
		if (entity->type == EntityType::ITEM) {
			Item* item = (Item*)entity;
			std::cout << "\nThere is an item here: " << item->name;
		}
	}

	// List creatures --
	for (Entity* entity : contains) {
		if (entity->type == EntityType::CREATURE || entity->type == EntityType::ENEMY) {
			Creature* cr = (Creature*)entity;
			std::cout << "\nThere is someone else here: " << cr->name;
			if (cr->IsAlive() == false) {
				std::cout << " (dead)";
			}
		}
	}
	std::cout << "\n";
}

Exit* Room::GetExit(const std::string& direction) const {
	for (const Entity* entity : contains) {
		if (entity->type == EntityType::EXIT) {
			Exit* exit = (Exit*)entity;
			if (exit->GetDirectionString(exit->GetDirectionFrom(this))  == direction) {
				return exit;
			}
		}
	}
	return nullptr;
}
