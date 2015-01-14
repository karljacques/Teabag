#ifndef physicsManager_h__
#define physicsManager_h__

#include "pch.h"
#include "../../component/physics/physicsComponent.h"
#include "../../core/event/eventListener.h"
#include "../../core/entity-component/componentManager.h"

class PhysicsManager : public EventListener, public ComponentManager<PhysicsComponent>
{
public:
	PhysicsManager( );
	~PhysicsManager();

	void update( double dt );
	void handle( Event* e );

	// Getters
	btDiscreteDynamicsWorld* getDiscreteDynamicsWorld(){ return mWorld; };

	void initComponent( PhysicsComponent* comp, btCollisionShape* shape, btScalar mass, float3 pos, Quat rot);
private:
	btDiscreteDynamicsWorld* mWorld;


};
#endif // physicsManager_h__