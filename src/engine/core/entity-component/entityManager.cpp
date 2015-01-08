#include "pch.h"
#include "entityManager.h"


EntityManager::EntityManager(void)
{
	mIDCount = 1;
}


EntityManager::~EntityManager(void)
{
}

EntID EntityManager::createEntity( )
{
	Entity* e = new Entity();
	e->ID = mIDCount;
	mIDCount++;
	mEntities.push_back(e);
	return e->ID;
}

void EntityManager::removeEntity( EntID ID )
{
	for( auto i=mEntities.begin(); i!=mEntities.end(); i++ )
		if( (*i)->ID == ID )
		{
			mEntities.erase( i );
			break;
		}
}

Entity* EntityManager::getByID(EntID ID)
{
	for( auto i=mEntities.begin(); i!=mEntities.end(); i++ )
		if( (*i)->ID == ID )
		{
			return (*i);
		}
	return nullptr;
}

bool EntityManager::entityExists(EntID ID)
{
	for( auto i=mEntities.begin(); i!=mEntities.end(); i++ )
		if( (*i)->ID == ID )
		{
			return true;
		}
	return false;
}
