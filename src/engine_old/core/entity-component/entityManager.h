#pragma once

#include "entity.h"
#include "../../manager/manager.h"
#include <typeindex>
#include "componentManager.h"
#include <unordered_map>

class EntityManager: public Manager
{
public:
	EntityManager(void);
	~EntityManager(void);

	EntID createEntity();
	void removeEntity( EntID ID );

	Entity* getByID( EntID ID );
	bool entityExists( EntID ID );

	void update( double dt );

	template <class T>
	T* createComponent( EntID ID )
	{
		T* comp = new T();
		comp->ID = ID;

		// Assign it to it's owner
		// See if we have an owner for this type of component
		if( mComponentOwners.count( typeid(T) ) > 0 )
		{
			mComponentOwners[typeid(T)]->add( comp );
		}

		return comp;
	}

	template <class T>
	void registerComponentManager( ComponentManager* mgr )
	{
		mComponentOwners[typeid(T)] = mgr;
	}

private:

	std::vector<Entity*> mEntities;
	EntID mIDCount; /* TODO cheap method of finding a ID. Might have to find a better way 
							if there are likely to be more than 2 million entities created during the
							lifetime of the game. */

	// Map components of different types to the manager which will own them.
	std::unordered_map<std::type_index, ComponentManager*> mComponentOwners;
};

