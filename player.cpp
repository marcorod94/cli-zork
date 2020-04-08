#include "exit.h"
#include "item.h"
#include "room.h"
#include "player.h"
#include <iostream>

Player::Player(const char* name, const char* description, Room* location) :
	Creature(name, description, location) {
	type = EntityType::PLAYER;
}

bool Player::Go(const std::vector<std::string>& args) {
	Exit* exit = GetLocation()->GetExit(args[1]);

	if (exit == nullptr) {
		std::cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if (exit->locked) {
		std::cout << "\nThe exit " << exit->name << " is locked.\n";
		return false;
	}

	std::cout << "\nYou take direction " << exit->GetNameFrom((Room*)parent) << "...\n";
	ChangeParentTo(exit->GetDestinationFrom((Room*)parent));
	parent->Look();

	return true;
}

void Player::Look(const std::vector<std::string>& args) const {
	if (args.size() > 1) {
		for (Entity* entity : parent->contains) {
			if ((entity->name == args[1]) || (entity->type == EntityType::EXIT && (args[1] == ((Exit*)entity)->GetNameFrom((Room*)parent))))
			{
				entity->Look();
				return;
			}
		}
		if (args[1] == "me") {
			Entity::Look();
		}
	}
	else {
		parent->Look();
	}
}

bool Player::Take(const std::vector<std::string>& args) {
	if (args.size() == 4) {
		Item* item = (Item*)parent->Find(args[3], EntityType::ITEM);
		if (item == nullptr) {
			item = (Item*)Find(args[3], EntityType::ITEM);
		}

		if (item == nullptr) {
			std::cout << "\nCannot find '" << args[3] << "' in this room or in your inventory.\n";
			return false;
		}

		Item* subitem = (Item*)item->Find(args[1], EntityType::ITEM);

		if (subitem == nullptr) {
			std::cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return false;
		}

		std::cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(this);
	} else if (args.size() == 2) {
		Item* item = (Item*)parent->Find(args[1], EntityType::ITEM);

		if (item == nullptr) {
			std::cout << "\nThere is no item here with that name.\n";
			return false;
		}

		std::cout << "\nYou take " << item->name << ".\n";
		item->ChangeParentTo(this);
	}

	return false;
}

bool Player::Drop(const std::vector<std::string>& args) {
	if (args.size() == 2) {
		Item* item = (Item*)Find(args[1], EntityType::ITEM);

		if (item == nullptr) {
			std::cout << "\nThere is no item on you with that name.\n";
			return false;
		}

		std::cout << "\nYou drop " << item->name << "...\n";
		item->ChangeParentTo(parent);

		return true;
	} else if (args.size() == 4) {
		Item* item = (Item*)Find(args[1], EntityType::ITEM);

		if (item == nullptr) {
			std::cout << "\nCan not find '" << args[1] << "' in your inventory.\n";
			return false;
		}

		Item* container = (Item*)parent->Find(args[3], EntityType::ITEM);

		if (container == nullptr) {
			container = (Item*)Find(args[3], EntityType::ITEM);
			std::cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
			return false;
		}

		std::cout << "\nYou put " << item->name << " into " << container->name << ".\n";
		item->ChangeParentTo(container);

		return true;
	}

	return false;
}

void Player::Inventory() const {
	std::list<Entity*> items;
	FindAll(EntityType::ITEM, items);

	if (items.size() == 0) {
		std::cout << "\nYou do not own any item.\n";
		return;
	}

	for (Entity* item : items) {
		if (item == weapon) {
			std::cout << "\n" << item->name << " (as weapon)";
		}
		else if (item == armour) {
			std::cout << "\n" << item->name << " (as armour)";
		} else {
			std::cout << "\n" << item->name;
		}
	}
	std::cout << "\n";
}

bool Player::Equip(const std::vector<std::string>& args) {
	Item* item = (Item*)Find(args[1], EntityType::ITEM);

	if (item == nullptr) {
		std::cout << "\nCannot find '" << args[1] << "' is not in your inventory.\n";
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
			std::cout << "\n" << item->name << " cannot be equipped.\n";
			return false;
	}

	std::cout << "\nYou equip " << item->name << "...\n";

	return true;
}

bool Player::UnEquip(const std::vector<std::string>& args) {
	if (!IsAlive()) {
		return false;
	}

	Item* item = (Item*)Find(args[1], EntityType::ITEM);

	if (item == nullptr) {
		std::cout << "\n" << item->name << " is not in your inventory.\n";
		return false;
	}

	if (item == weapon) {
		weapon = nullptr;
	} else if (item == armour) {
		armour = nullptr;
	} else {
		std::cout << "\n" << item->name << " is not equipped.\n";
		return false;
	}

	std::cout << "\nYou un-equip " << item->name << "...\n";

	return true;
}

bool Player::Examine(const std::vector<std::string>& args) const {
	Creature* target = FindTarget(args);

	if (target == nullptr) {
		std::cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	target->Inventory();
	target->Stats();

	return true;
}

bool Player::Attack(const std::vector<std::string>& args) {
	Creature* target = FindTarget(args);

	if (target == nullptr) {
		std::cout << "\n" << args[1] << " is not here.";
		return false;
	}

	combat_target = target;
	std::cout << "\nYou jump to attack " << target->name << "!\n";
	return true;
}

bool Player::Loot(const std::vector<std::string>& args) {
	Creature* target = FindTarget(args);

	if (target == nullptr) {
		std::cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	if (target->IsAlive() == true) {
		std::cout << "\n" << target->name << " cannot be looted until it is killed.\n";
		return false;
	}

	std::list<Entity*> items;
	target->FindAll(EntityType::ITEM, items);

	if (items.size() > 0) {
		std::cout << "\nYou loot " << target->name << "'s corpse:\n";

		for (Entity* entity : items) {
			Item* item = (Item*)(entity);
			std::cout << "You find: " << item->name << "\n";
			item->ChangeParentTo(this);
		}
	} else {
		std::cout << "\nYou loot " << target->name << "'s corpse, but find nothing there.\n";
	}

	return true;
}

bool Player::Lock(const std::vector<std::string>& args) {
	if (!IsAlive()) {
		return false;
	}

	Exit* exit = GetLocation()->GetExit(args[1]);

	if (exit == nullptr) {
		std::cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if (exit->locked == true) {
		std::cout << "\nThat exit is already locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], EntityType::ITEM);

	if (item == nullptr) {
		std::cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if (exit->key != item) {
		std::cout << "\nItem '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	std::cout << "\nYou lock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = true;

	return true;
}

bool Player::UnLock(const std::vector<std::string>& args) {
	if (!IsAlive())
		return false;

	Exit* exit = GetLocation()->GetExit(args[1]);

	if (exit == nullptr) {
		std::cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if (exit->locked == false) {
		std::cout << "\nThat exit is not locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], EntityType::ITEM);

	if (item == nullptr) {
		std::cout << "\nKey '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if (exit->key != item) {
		std::cout << "\nKey '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	std::cout << "\nYou unlock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = false;

	return true;
}

Creature* Player::FindTarget(const std::vector<std::string>& args) const{
	Creature* target = (Creature*)parent->Find(args[1], EntityType::CREATURE);
	if (target == nullptr) {
		target = (Creature*)parent->Find(args[1], EntityType::ENEMY);
	}
	return target;
}
