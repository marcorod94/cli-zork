#include "common.h"
#include "exit.h"
#include "item.h"
#include "npc.h"
#include "player.h"
#include "room.h"
#include "world.h"
#include <iostream>

World::World() {
	tick_timer = clock();
	CreateRooms();
	CreateExits();
	CreateEnemies();
	CreateItems();
	CreatePlayer();
}

World::~World() {
	for (Entity* entity : entities) {
		delete entity;
	}
	entities.clear();
}

bool World::Update(std::vector<std::string>& args) {
	bool ret = true;

	if (args.size() > 0 && args[0].length() > 0) {
		ret = ParseCommand(args);
	}
	GameLoop();
	return ret;
}

bool World::ParseCommand(std::vector<std::string>& args) {
	bool ret = true;

	switch (args.size()) {
		case 1:
			ret = HandleOneWordInput(args);
			break;
		case 2:
			ret = HandleTwoWordInput(args);
			break;
		case 4:
			ret = HandleFourWordInput(args);
			break;
		default:
			ret = false;
	}
	return ret;
}

void World::GameLoop() {
	clock_t now = clock();

	if ((now - tick_timer) / CLOCKS_PER_SEC > UPDATE_FREQUENCY)
	{
		for (Entity* entity : entities) {
			entity->Update();
		}
		tick_timer = now;
	}
}

bool World::HandleOneWordInput(std::vector<std::string>& args) {
	bool ret = true;
	if (Compare(args[0], "look") || Compare(args[0], "l")) {
		player->Look(args);
	} else if (Compare(args[0], "stats") || Compare(args[0], "st")) {
		player->Stats();
	} else if (Compare(args[0], "inventory") || Compare(args[0], "i")) {
		player->Inventory();
	} else if (Compare(args[0], "north") || Compare(args[0], "n")) {
		MovePlayer(args, "north");
	} else if (Compare(args[0], "south") || Compare(args[0], "s")) {
		MovePlayer(args, "south");
	} else if (Compare(args[0], "east") || Compare(args[0], "e")) {
		MovePlayer(args, "east");
	} else if (Compare(args[0], "west") || Compare(args[0], "w")) {
		MovePlayer(args, "west");
	} else if (Compare(args[0], "up") || Compare(args[0], "u")) {
		MovePlayer(args, "up");
	} else if (Compare(args[0], "down") || Compare(args[0], "d")) {
		MovePlayer(args, "down");
	} else {
		ret = false;
	}
	return ret;
}

bool World::HandleTwoWordInput(std::vector<std::string>& args) {
	bool ret = true;
	if (Compare(args[0], "look") || Compare(args[0], "l")) {
		player->Look(args);
	} else if (Compare(args[0], "go")) {
		player->Go(args);
	} else if (Compare(args[0], "take") || Compare(args[0], "pick")) {
		player->Take(args);
	} else if (Compare(args[0], "drop") || Compare(args[0], "put")) {
		player->Drop(args);
	} else if (Compare(args[0], "equip") || Compare(args[0], "eq")) {
		player->Equip(args);
	} else if (Compare(args[0], "unequip") || Compare(args[0], "uneq")) {
		player->UnEquip(args);
	} else if (Compare(args[0], "examine") || Compare(args[0], "ex")) {
		player->Examine(args);
	} else if (Compare(args[0], "attack") || Compare(args[0], "at")) {
		player->Attack(args);
	} else if (Compare(args[0], "loot") || Compare(args[0], "lt")) {
		player->Loot(args);
	} else {
		ret = false;
	}
	return ret;
}

bool World::HandleFourWordInput(std::vector<std::string>& args) {
	bool ret = true;
	if (Compare(args[0], "unlock") || Compare(args[0], "unlk")) {
		player->UnLock(args);
	} else if (Compare(args[0], "lock") || Compare(args[0], "lk")) {
		player->Lock(args);
	} else if (Compare(args[0], "take") || Compare(args[0], "pick")) {
		player->Take(args);
	} else if (Compare(args[0], "drop") || Compare(args[0], "put")) {
		player->Drop(args);
	} else {
		ret = false;
	}
	return ret;
}

void World::MovePlayer(std::vector<std::string>& args, const char* param) {
	if (args.size() == 1) {
		args.push_back(param);
	} else {
		args[1] = param;
	}
	player->Go(args);
}

void World::CreateRooms() {
	forest = new Room("Forest", "You are surrounded by tall trees. It feels like a huge forest someone could get lost easily.");
	house = new Room("House", "You are inside a beautiful but small white house.");
	basement = new Room("Basement", "The basement features old furniture and dim light.");
	clearing = new Room("Clearing", "You are in a huge clearing.");
	forest2 = new Room("Forest", "You are surrounded by tall trees. It feels like a huge forest someone could get lost easily.");
	forest3 = new Room("Forest", "You are surrounded by tall trees. It feels like a huge forest someone could get lost easily.");

	entities.push_back(forest);
	entities.push_back(forest2);
	entities.push_back(forest3);
	entities.push_back(clearing);
	entities.push_back(house);
	entities.push_back(basement);
}

void World::CreateExits() {
	front_door = new Exit("Front_door", "Little path", forest, house);
	front_door->SetDirections(Direction::east, Direction::west);

	basement_door = new Exit("Basement_door", "Stairs", house, basement);
	basement_door->SetDirections(Direction::down, Direction::up);
	basement_door->locked = true;

	back_door = new Exit("Back_door", "Door", house, clearing);
	back_door->SetDirections(Direction::east, Direction::west);

	forest_path = new Exit("Forest_Path", "Door", forest, forest2);
	forest_path->SetDirections(Direction::north, Direction::south);

	forest_path2 = new Exit("Forest_Path", "Door", forest2, forest3);
	forest_path2->SetDirections(Direction::east, Direction::west);

	forest_path3 = new Exit("Forest_Path", "Door", forest3, clearing);
	forest_path3->SetDirections(Direction::south, Direction::north);
	
	entities.push_back(front_door);
	entities.push_back(basement_door);
	entities.push_back(back_door);
	entities.push_back(forest_path);
	entities.push_back(forest_path2);
	entities.push_back(forest_path3);
}

void World::CreateEnemies() {
	butler = new NPC("Butler", "It's James, the house Butler.", house);
	butler->hit_points = 10;
	entities.push_back(butler);

	wolf = new NPC("Wolf", "It's a angry wolf.", forest2);
	wolf->hit_points = 15;
	entities.push_back(wolf);
}

void World::CreateItems() {
	Item* mailbox = new Item("Mailbox", "Looks like it might contain something.", house);
	Item* key = new Item("Key", "Old iron key.", mailbox);
	basement_door->key = key;

	Item* sword = new Item("Sword", "A simple old and rusty sword.", forest, ItemType::WEAPON);
	sword->min_value = 2;
	sword->max_value = 6;

	Item* sword2(sword);
	sword2->parent = butler;

	Item* fang = new Item("Fang", "Pretty sharpe fang", wolf, ItemType::WEAPON);
	fang->min_value = 3;
	fang->max_value = 7;
	wolf->AutoEquip();

	Item* shield = new Item("Shield", "An old wooden shield.", butler, ItemType::ARMOUR);
	shield->min_value = 1;
	shield->max_value = 3;
	butler->AutoEquip();

	entities.push_back(mailbox);
	entities.push_back(sword);
	entities.push_back(shield);
	entities.push_back(fang);
}

void World::CreatePlayer() {
	player = new Player("Hero", "You are an awesome adventurer!", forest);
	player->hit_points = 25;
	entities.push_back(player);
}
