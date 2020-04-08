#pragma once
#include "entity.h"

enum class ItemType {
	ARMOUR,
	COMMON,
	WEAPON
};

class Item : public Entity {
public:
	int min_value;
	int max_value;
	ItemType item_type;

	Item(const char* name, const char* description, Entity* parent, ItemType item_type = ItemType::COMMON);
	void Look() const;
	int GetValue() const;
};
