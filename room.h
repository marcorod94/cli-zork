#pragma once
#include "entity.h"

class Exit;

class Room : public Entity {
public:
	Room(const char* name, const char* description);
	void Look() const;
	Exit* GetExit(const std::string& direction) const;
};
