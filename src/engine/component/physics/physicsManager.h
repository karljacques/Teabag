#ifndef physicsManager_h__
#define physicsManager_h__

/* includes go:
-Components
-Managers
-Core
*/

#include "pch.h"

#include "physicsComponent.h"

#include "engine/manager.h"
#include "engine/component/componentManager.h"

#include "engine/core/eventSystem.h"
#include "engine/core/physicsSystem.h"

class PhysicsManager : public EventListener, public ComponentManager, public Manager
{
public:
	PhysicsManager( );
	~PhysicsManager();

	void update( double dt );
	void handle( Event* e );

	void initComponent( PhysicsComponent* comp, btCollisionShape* shape, btScalar mass, float3 pos, Quat rot);
	void deinitComponent( Component* comp );
};
#endif // physicsManager_h__