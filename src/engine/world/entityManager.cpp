#include "pch.h"
#include "entityManager.h"


EntityManager::EntityManager(void)
{
	mLUIDCount = 0;
}


EntityManager::~EntityManager(void)
{
}

Entity* EntityManager::createEntity( )
{
	Entity* e = new Entity();
	e->LUID = mLUIDCount;
	mLUIDCount++;
	mEntities.push_back(e);
	return e;
}

void EntityManager::removeEntity( unsigned int LUID )
{
	for( auto i=mEntities.begin(); i!=mEntities.end(); i++ )
		if( (*i)->LUID == LUID )
		{
			mEntities.erase( i );
			break;
		}
}

Entity* EntityManager::getByLUID(unsigned int LUID)
{
	for( auto i=mEntities.begin(); i!=mEntities.end(); i++ )
		if( (*i)->LUID == LUID )
		{
			return (*i);
		}
	return nullptr;
}

bool EntityManager::entityExists(unsigned int LUID)
{
	for( auto i=mEntities.begin(); i!=mEntities.end(); i++ )
		if( (*i)->LUID == LUID )
		{
			return true;
		}
	return false;
}
