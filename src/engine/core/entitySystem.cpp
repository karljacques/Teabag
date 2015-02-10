#include "pch.h"
#include "entitySystem.h"

static std::vector<Entity*> entities;
static EntID mIDCount = 1; /* TODO cheap method of finding a ID. Might have to implement a better way 
							if there are likely to be more than 2 million entities created during the
							lifetime of the game. */

// Map components of different types to the manager which will own them.
// TODO: Needs to be a multimap - there may be multiple managers that want the same component
std::unordered_map<std::type_index, ComponentManager*> componentOwners;

EntID entitysys::createEntity( void )
{
	Entity* e = new Entity();
	e->ID = mIDCount;
	mIDCount++;
	entities.push_back(e);
	printm( "Created entity with ID:" + std::to_string( e->ID ));
	return e->ID;
}

void entitysys::destroyEntity( Entity* ent )
{
	auto i=ent->mComponents.begin();
	while( i!=ent->mComponents.end() )
	{
		entitysys::destroyComponent(*(i++));
	}

	for( auto i=entities.begin(); i!=entities.end(); i++ )
	{
		if( (*i)->ID == ent->ID )
		{
			entities.erase( i );
			break;
		}
	}

	delete ent;
}

Entity* entitysys::getByID(EntID ID)
{
	for( auto i=entities.begin(); i!=entities.end(); i++ )
		if( (*i)->ID == ID )
		{
			return (*i);
		}
	return nullptr;
}

bool entitysys::entityExists(EntID ID)
{
	for( auto i=entities.begin(); i!=entities.end(); i++ )
		if( (*i)->ID == ID )
		{
			return true;
		}
	return false;
}

void entitysys::destroyComponent( Component* comp )
{
	std::type_index type = typeid( *comp );

	if( componentOwners.count( type  ) > 0 )
	{
		componentOwners[type]->removeComponent(comp->ID);
		componentOwners[type]->deinitComponent(comp);
	}

	entitysys::getByID(comp->ID)->removeComponent(comp);

	delete comp;
}