#ifndef physicsManager_h__
#define physicsManager_h__

#include <btBulletDynamicsCommon.h>
#include "engine/core/component/base/component.h"
#include "engine/core/component/physics/physicsComponent.h"
#include "engine/core/event/eventListener.h"
#include "engine/core/componentManager.h"

class PhysicsManager : public EventListener, public ComponentManager<PhysicsComponent>
{
public:
	PhysicsManager( EntityManager* ent );
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