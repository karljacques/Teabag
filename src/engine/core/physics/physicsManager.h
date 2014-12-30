#ifndef physicsManager_h__
#define physicsManager_h__

#include <btBulletDynamicsCommon.h>
#include "engine/core/component/base/component.h"
#include "engine/core/component/physics/physicsComponent.h"
#include "engine/core/event/eventListener.h"

class PhysicsManager : public EventListener
{
public:
	PhysicsManager( EntityManager* ent );
	~PhysicsManager();

	void update( double dt );
	void handle( Event* e );

	// Getters
	btDiscreteDynamicsWorld* getDiscreteDynamicsWorld(){ return mWorld; };

	Component* createComponent( btCollisionShape* shape, btScalar* mass, float3 pos, Quat rot);
private:
	btDiscreteDynamicsWorld* mWorld;

	// List of all physics components
	std::list<PhysicsComponent*> mComponents;

};
#endif // physicsManager_h__