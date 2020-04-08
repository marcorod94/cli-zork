#pragma once
#include "entity.h"
#include <vector>
class Room;
class Item;

class Creature : public Entity {
public:

	int hit_points;
	int min_damage;
	int max_damage;
	int min_protection;
	int max_protection;
	Creature* combat_target;
	Item* weapon;
	Item* armour;
	Room* location;

	Creature(const char* name, const char* description, Room* location);
	virtual bool Go(const std::vector<std::string>& args);
	virtual void Look(const std:: vector<std::string>& args) const;
	virtual bool Take(const std:: vector<std::string>& args);
	virtual bool Drop(const std:: vector<std::string>& args);
	virtual void Inventory() const;
	virtual bool Equip(const std:: vector<std::string>& args);
	virtual bool UnEquip(const std:: vector<std::string>& args);
	virtual bool AutoEquip();
	virtual bool Lock(const std:: vector<std::string>& args);
	virtual bool UnLock(const std:: vector<std::string>& args);
	virtual void Update();

	virtual bool Attack(const std:: vector<std::string>& args);
	virtual int MakeAttack();
	virtual int ReceiveAttack(int damage);
	virtual void Die();
	virtual bool Loot(const std:: vector<std::string>& args);
	virtual void Stats() const;

	Room* GetLocation() const;
	bool PlayerInRoom() const;
	bool IsAlive() const;

};
