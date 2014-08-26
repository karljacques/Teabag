#include "pch.h"
#include "entity.h"


Entity::Entity(void)
{

}


Entity::~Entity(void)
{
}

void Entity::addComponent( Component* component )
{
	mComponents.push_back(component);
}

void Entity::update( double dt )
{
	for( auto i=mComponents.begin(); i!=mComponents.end(); i++ )
		(*i)->update( dt );
}