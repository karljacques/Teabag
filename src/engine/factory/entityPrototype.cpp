#include "pch.h"
#include "entityPrototype.h"

EntityPrototype::EntityPrototype( Entity* ent) 
{
	_prototype = _clone( ent );

	// Deactivate the prototype
	for( auto i=_prototype->mComponents.begin();i!=_prototype->mComponents.end();i++)
	{
		(*i)->deactivate();
	}

}

Entity* EntityPrototype::spawn(void)
{
	Entity* ent	= _clone( _prototype );
	
	// Activate all components
	for( auto i=ent->mComponents.begin();i!=ent->mComponents.end();i++)
	{
		(*i)->activate();
	}

	return ent;
}

Entity* EntityPrototype::_clone(Entity* ent)
{
	// Create new entity
	EntID ID = entitysys::createEntity();
	Entity* newEnt = entitysys::getByID(ID);

	// Clone components to new entity
	for( auto i=ent->mComponents.begin(); i!=ent->mComponents.end(); i++ )
	{
		newEnt->addComponent( (*i)->clone(ID) );
	}

	return newEnt;
}

