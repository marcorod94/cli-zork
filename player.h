#pragma once
#include "creature.h"
#include <vector>

class Player : public Creature {
public:
	Player(const char* name, const char* description, Room* location);
	bool Go(const std::vector<std::string>& args);
	void Look(const std::vector<std::string>& args) const;
	bool Take(const std::vector<std::string>& args);
	bool Drop(const std::vector<std::string>& args);
	void Inventory() const;
	bool Equip(const std::vector<std::string>& args);
	bool UnEquip(const std::vector<std::string>& args);
	bool Examine(const std::vector<std::string>& args) const;
	bool Attack(const std::vector<std::string>& args);
	bool Loot(const std::vector<std::string>& args);
	bool Lock(const std::vector<std::string>& args);
	bool UnLock(const std::vector<std::string>& args);
	
private: 
	Creature* FindTarget(const std::vector<std::string>& args) const;
};
