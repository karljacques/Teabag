#include "pch.h"
#include "entity.h"


Entity::Entity(void)
{
	mPosition = Ogre::Vector3(0,0,0);

	physicsComponent = 0;
	renderComponent = 0;
}


Entity::~Entity(void)
{
}

void Entity::update( double dt )
{
	// If components are set, update them
	if( physicsComponent )
		physicsComponent->update( dt );

	if( renderComponent )
		renderComponent->update( dt );
}