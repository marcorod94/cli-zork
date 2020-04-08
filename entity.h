#pragma once
#include <list>
#include <string>


enum class EntityType {
	CREATURE,
	ENEMY,
	ENTITY,
	EXIT,
	ITEM,
	PLAYER,
	ROOM
};

class Entity {
public:
	//Variables
	EntityType type;
	std::string name;
	std::string description;
	Entity* parent;
	std::list<Entity*> contains;

	//Methods
	Entity(const char* name, const char* description, Entity* parent);	
	virtual void Look() const;
	virtual void Update();
	void	ChangeParentTo(Entity* new_parent);
	bool    Find(Entity* entity) const;
	Entity* Find(EntityType type) const;
	Entity* Find(const std::string& name, EntityType type) const;
	void	FindAll(EntityType type, std::list<Entity*>& list_copy) const;
};
