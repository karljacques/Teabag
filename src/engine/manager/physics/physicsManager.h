#ifndef physicsManager_h__
#define physicsManager_h__

#include "pch.h"
#include "../../component/physics/physicsComponent.h"
#include "../../core/event/eventListener.h"
#include "../../core/entity-component/componentManager.h"

using namespace std;

class PhysicsManager : public EventListener, public ComponentManager<PhysicsComponent>
{
public:
	PhysicsManager( );
	~PhysicsManager();

	void update( double dt );
	void handle( Event* e );

	// Getters
	//btDiscreteDynamicsWorld* getDiscreteDynamicsWorld(){ return mWorld.get(); };

	void initComponent( PhysicsComponent* comp, btCollisionShape* shape, btScalar mass, float3 pos, Quat rot);
private:
	
	unique_ptr<btBroadphaseInterface> mBroadphase;
	unique_ptr<btCollisionDispatcher> mDispatcher;
	unique_ptr<btDefaultCollisionConfiguration> mCollisionConfiguration;
	unique_ptr<btSequentialImpulseConstraintSolver> mSolver;

	unique_ptr<btDiscreteDynamicsWorld> mWorld;
};
#endif // physicsManager_h__