#ifndef entityManager_h__
#define entityManager_h__

#include "entity/entity.h"
#include "engine/manager.h"
#include "engine/component/componentManager.h"
#include "engine/manager/debug/ogreConsole.h"

/* Don't touch these. They're for the templated functions only.
 **Insert threat here**
 */
extern std::unordered_map<std::type_index, ComponentManager*> componentOwners;

namespace entitysys
{
	EntID createEntity( void );
	void destroyEntity( Entity* ent );

	Entity* getByID( EntID ID );
	bool entityExists( EntID ID );

	template <class T>
	T* createComponent( EntID ID)
	{
		T* comp = new T();
		comp->ID = ID;

		// Assign it to it's owner
		// See if we have an owner for this type of component
		if( componentOwners.count( typeid(T) ) > 0 )
		{
			componentOwners[typeid(T)]->add( comp );
		}

		return comp;
	}

	// Unregisters component and deletes it
	void destroyComponent(  Component* comp );

	template <class T>
	void registerComponentManager( ComponentManager* mgr )
	{
		componentOwners[typeid(T)] = mgr;
	}

	template <class T>
	ComponentManager* getComponentManager()
	{
		return componentOwners[typeid(T)];
	}

	ComponentManager* getComponentManager( Component* comp );
}


#endif // entityManager_h__
