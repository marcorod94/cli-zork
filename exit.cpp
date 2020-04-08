#include "entity.h"
#include "exit.h"
#include "room.h"

Exit::Exit(const char* name, const char* description, Room* source, Room* destination, bool one_way) :
	Entity(name, description, (Entity*) source),
	closed(false), locked(false), key(NULL), one_way(one_way), destination(destination) {
	type = EntityType::EXIT;
	if (!one_way) {
		this->destination->contains.push_back(this);
	}
}

const Direction& Exit::GetDirectionFrom(const Room* room) const {
	if (room == parent) {
		return direction;
	}
	if (room == destination) {
		return opposite_direction;
	}
	return direction;
}

const std::string Exit::GetNameFrom(const Room* room) const {
	if (room == parent) {
		return GetDirectionString(direction);
	}
	if (room == destination) {
		return GetDirectionString(opposite_direction);
	}
	return GetDirectionString(direction);
}

Room* Exit::GetDestinationFrom(const Room* room) const {
	if (room == parent) { return destination; }
	if (room == destination) { return (Room*)parent; }
	return nullptr;
}

void Exit::SetDirections(Direction direction, Direction opposite_direction) {
	this->direction = direction;
	this->opposite_direction = opposite_direction;
}

const std::string Exit::GetDirectionString(Direction direction) const
{	
	switch (direction) {
		case Direction::down:
			return "down";
			break;
		case Direction::up:
			return "up";
			break;
		case Direction::east:
			return "east";
			break;
		case Direction::west:
			return "west";
			break;
		case Direction::north:
			return"north";
			break;
		case Direction::south:
			return "south";
			break;
	}
	return "";
}
