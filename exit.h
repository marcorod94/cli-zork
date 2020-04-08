#pragma once
#include "entity.h"
#include <string>

class Room;

enum class Direction {
	down,
	east,
	north,
	south,
	up,
	west
};

class Exit : public Entity {
public:
	bool one_way;
	bool closed;
	bool locked;
	Direction direction;
	Direction opposite_direction;
	Room* source;
	Room* destination;
	Entity* key;

	Exit(const char* name, const char* description, Room* source, Room* destination, bool one_way = false);
	const Direction& GetDirectionFrom(const Room* room) const;
	const std::string GetNameFrom(const Room* room) const;
	Room* GetDestinationFrom(const Room* room) const;
	void SetDirections(Direction direction, Direction opposite_direction);
	const std::string GetDirectionString(Direction direction) const;
};
