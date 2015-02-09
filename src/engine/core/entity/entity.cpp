#include "pch.h"
#include "entity.h"


Entity::Entity(void)
{
	ID = 0;
}


Entity::~Entity(void)
{
}

void Entity::addComponent( Component* component )
{
	mComponents.push_back(component);
}

void Entity::removeComponent( Component* c )
{
	mComponents.remove(c);
}