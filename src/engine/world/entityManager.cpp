#include "pch.h"
#include "entityManager.h"


EntityManager::EntityManager(void)
{
}


EntityManager::~EntityManager(void)
{
}

void EntityManager::addEntity( Entity* e )
{
	mEntities.push_back(e);
}

void EntityManager::removeEntity( Entity* e )
{
	for( auto i=mEntities.begin(); i!=mEntities.end(); i++ )
		if( (*i) == e )
		{
			mEntities.erase( i );
			break;
		}
}
