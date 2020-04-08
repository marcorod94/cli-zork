#include "common.h"
#include "creature.h"
#include "exit.h"
#include "item.h"
#include "room.h"
#include <iostream>

Creature::Creature(const char* name, const char* description, Room* location) :
	Entity(name, description, (Entity*)location) {
	type = EntityType::CREATURE;
}

bool Creature::Go(const std::vector<std::string>& args) {
	if (!IsAlive()) {
		return false;
	}

	Exit* exit = GetLocation()->GetExit(args[1]);

	if (exit == nullptr) {
		return false;
	}

	if (PlayerInRoom()) {
		std::cout << name << "goes " << args[1] << "...\n";
	}

	ChangeParentTo(exit->GetDestinationFrom((Room*)parent));

	return true;
}

void Creature::Look(const std::vector<std::string>& args) const {
	if (IsAlive()) {
		Entity::Look();
	} else {
		std::cout << name << "'s corpse\n";
		std::cout << "Here lies dead: " << description << "\n";
	}
}

bool Creature::Take(const std::vector<std::string>& args) {
	if (!IsAlive()) {
		return false;
	}

	Item* item = (Item*)parent->Find(args[1], EntityType::ITEM);

	if (args.size() > 1) {
		// we could pick something from a container in our inventory ...
		if (item == nullptr) {
			item = (Item*)Find(args[1], EntityType::ITEM);
		}

		if (item == nullptr) {
			return false;
		}

		Item* subitem = (Item*)item->Find(args[3], EntityType::ITEM);

		if (subitem == nullptr) {
			return false;
		}

		if (PlayerInRoom()) {
			std::cout << name << " looks into " << item->name << "...\n";
		}
		item = subitem;
	}

	if (item == nullptr) {
		return false;
	}

	if (PlayerInRoom()) {
		std::cout << name << " takes " << item->name << ".\n";
	}
	item->ChangeParentTo(this);

	return true;
}

bool Creature::Drop(const std::vector<std::string>& args) {
	if (!IsAlive()) {
		return false;
	}

	Item* item = (Item*)Find(args[1], EntityType::ITEM);

	if (item == nullptr) {
		return false;
	}

	if (PlayerInRoom()) {
		std::cout << name << " drops " << item->name << "...\n";
	}
	item->ChangeParentTo(parent);

	return true;
}

void Creature::Inventory() const {
	std::list<Entity*> items;
	FindAll(EntityType::ITEM, items);

	if (items.size() == 0) {
		std::cout << name << " does not own any items\n";
		return;
	}

	std::cout << "\n" << name << " owns:\n";
	for (Entity* entity : items) {
		if (entity == weapon) {
			std::cout << entity->name << " (as weapon)\n";
		} else if (entity == armour) {
			std::cout << entity->name << " (as armour)\n";
		} else {
			std::cout << entity->name << "\n";
		}
	}
}

bool Creature::Equip(const std::vector<std::string>& args) {
	if (!IsAlive()) {
		return false;
	}

	Item* item = (Item*)Find(args[1], EntityType::ITEM);

	if (item == nullptr) {
		return false;
	}

	switch (item->item_type) {
		case ItemType::WEAPON:
			weapon = item;
			break;

		case ItemType::ARMOUR:
			armour = item;
			break;

		default:
			return false;
	}

	if (PlayerInRoom()) {
		std::cout << name << " equips " << item->name << "...\n";
	}

	return true;
}

bool Creature::UnEquip(const std::vector<std::string>& args) {
	if (!IsAlive()) {
		return false;
	}

	Item* item = (Item*)Find(args[1], EntityType::ITEM);

	if (item == nullptr) {
		return false;
	}

	if (item == weapon) {
		weapon = nullptr;
	} else if (item == armour) {
		armour = nullptr;
	} else {
		return false;
	}

	if (PlayerInRoom()) {
		std::cout << name << " un-equips " << item->name << "...\n";
	}

	return true;
}

bool Creature::AutoEquip() {
	if (!IsAlive()) {
		return false;
	}

	std::list<Entity*> items;
	FindAll(EntityType::ITEM, items);

	for (Entity* entity : items) {
		Item* item = (Item*)entity;
		if (item->item_type == ItemType::WEAPON) {
			weapon = item;
		}
		if (item->item_type == ItemType::ARMOUR) {
			armour = item;
		}
	}
	return true;
}

bool Creature::Lock(const std::vector<std::string>& args) {
	if (!IsAlive()) {
		return false;
	}

	Exit* exit = GetLocation()->GetExit(args[1]);

	if (exit == nullptr || exit->locked == true) {
		return false;
	}

	Item* item = (Item*)Find(args[3], EntityType::ITEM);

	if (item == nullptr || exit->key != item) {
		return false;
	}

	if (PlayerInRoom()) {
		std::cout << "\n" << name << "locks " << exit->GetNameFrom((Room*)parent) << "...\n";
	}

	exit->locked = true;

	return true;
}

bool Creature::UnLock(const std::vector<std::string>& args) {
	if (!IsAlive())
		return false;

	Exit* exit = GetLocation()->GetExit(args[1]);

	if (exit == nullptr || exit->locked == false) {
		return false;
	}

	Item* item = (Item*)Find(args[3], EntityType::ITEM);

	if (item == nullptr || exit->key != item) {
		return false;
	}

	if (PlayerInRoom()) {
		std::cout << "\n" << name << "unlocks " << exit->GetNameFrom((Room*)parent) << "...\n";
	}

	exit->locked = false;

	return true;
}

void Creature::Update() {
	if (combat_target != nullptr) {
		if (parent->Find(combat_target) == true) {
			MakeAttack();
		}
		else {
			combat_target = nullptr;
		}
	}
}

bool Creature::Attack(const std::vector<std::string>& args) {
	Creature *target = (Creature*)parent->Find(args[1], EntityType::CREATURE);

	if (target == nullptr) {
		return false;
	}
	combat_target = target;
	std::cout << "\n" << name << " attacks " << target->name << "!\n";
	return true;
}

int Creature::MakeAttack() {
	if (!IsAlive() || !combat_target->IsAlive()) {
		combat_target = combat_target->combat_target = nullptr;
		return false;
	}

	int result = (weapon) ? weapon->GetValue() : GetRandomValue(min_damage, max_damage);

	if (PlayerInRoom()) {
		std::cout << name << " attacks " << combat_target->name << " for " << result << "\n";
	}

	combat_target->ReceiveAttack(result);

	// make the attacker react and take me as a target
	if (combat_target->combat_target == nullptr) {
		combat_target->combat_target = this;
	}
	return result;
}

int Creature::ReceiveAttack(int damage) {
	int prot = (armour) ? armour->GetValue() : GetRandomValue(min_protection, max_protection);
	int received = damage - prot;

	hit_points -= received;

	if (PlayerInRoom())
		std::cout << name << " is hit for " << received << " damage (" << prot << " blocked) \n";

	if (IsAlive() == false)
		Die();

	return received;
}

void Creature::Die() {
	if (PlayerInRoom()) {
		std::cout << name << " dies.\n";
	}
}

bool Creature::Loot(const std::vector<std::string>& args) {
	Creature *target = (Creature*)parent->Find(args[1], EntityType::CREATURE);

	if (target == nullptr && target->IsAlive() == false) {
		return false;
	}

	std::list<Entity*> items;
	target->FindAll(EntityType::ITEM, items);

	for (Entity* entity : items) {
		Item* item = (Item*)entity;
		item->ChangeParentTo(this);
	}

	std::cout << "\n" << name << " loots " << target->name << "'s corpse\n";

	return true;
}

void Creature::Stats() const {
	std::cout << "\nHit Points: " << hit_points;
	std::cout << "\nAttack: (" << ((weapon) ? weapon->name : "no weapon") << ") ";
	std::cout << ((weapon) ? weapon->min_value : min_damage) << "-" << ((weapon) ? weapon->max_value : max_damage);
	std::cout << "\nProtection: (" << ((armour) ? armour->name : "no armour") << ") ";
	std::cout << ((armour) ? armour->min_value : min_protection) << "-" << ((armour) ? armour->max_value : max_protection);
	std::cout << "\n";
}

Room* Creature::GetLocation() const {
	return (Room*) parent;
}

bool Creature::PlayerInRoom() const {
	return parent->Find(EntityType::PLAYER) != nullptr;
}

bool Creature::IsAlive() const {
	return hit_points > 0;
}
