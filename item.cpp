#include "common.h"
#include "item.h"
#include<iostream>

Item::Item(const char* name, const char* description, Entity* parent, ItemType item_type) :
	Entity(name, description, parent), item_type(item_type) {
	type = EntityType::ITEM;
	min_value = max_value = 0;
}

void Item::Look() const {
	Entity::Look();
	std::list<Entity*> items;
	FindAll(EntityType::ITEM, items);

	if (items.size() > 0) {
		std::cout << "It contains: " << "\n";
		for (Entity* item : items) {
			std::cout << item->name << "\n";
		}
	}
}

int Item::GetValue() const {
	return GetRandomValue(min_value, max_value);
}
