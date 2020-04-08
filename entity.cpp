#include "entity.h"
#include <iostream>

Entity::Entity(const char* name, const char* description, Entity* parent = nullptr) :
	name(name), description(description), parent(parent) {
	type = EntityType::ENTITY;
	if (parent != nullptr) {
		parent->contains.push_back(this);
	}	
}

void Entity::Update() {
}

void Entity::Look() const {
	std::cout << "\n" << name << "\n";
	std::cout << description << "\n";
}

void Entity::ChangeParentTo(Entity* new_parent) {
	if (parent != NULL)
		parent->contains.remove(this);

	parent = new_parent;

	if (parent != NULL)
		parent->contains.push_back(this);
}

bool Entity::Find(Entity* entity) const {
	for (Entity* ent : contains) {
		if (ent == entity) {
			return true;
		}
	}
	return false;
}

Entity* Entity::Find(EntityType type) const {
	for (Entity* entity : contains) {
		if (entity->type == type)
			return entity;
	}
	return nullptr;
}

Entity* Entity::Find(const std::string & name, EntityType type) const {
	for (Entity* entity : contains) {
		if (entity->type == type && entity->name == name) {			
				return entity;
		}
	}
	return nullptr;
}

void Entity::FindAll(EntityType type, std::list<Entity*>& list_copy) const {
	for (Entity* entity : contains) {
		if (entity->type == type) {
			list_copy.push_back(entity);
		}
	}
}
