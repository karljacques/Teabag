#ifndef physicsComponent_h__
#define physicsComponent_h__

#include "engine/core/component/base/component.h"
#include "engine/core/physics/physicsManager.h"
#include "engine/core/component/base/positionComponent.h"
#include "engine/core/event/eventListener.h"
#include "common.h"

class PhysicsComponent :
	public Component,public EventListener
{
public:
	PhysicsComponent( PhysicsManager* physicsManager, PositionComponent* positionComponent );
	~PhysicsComponent(void);

	void setAsBox( float x, float y, float z );
	void update(  double dt );
	void handle( Event* e );

	void initialise( btCollisionShape* shape, btScalar mass, float3 position, Quat orientation = Quat(0,0,0,1) );


	// Getters
	btRigidBody* getBody(){ return mBody; };
	
private:
	PhysicsManager* mPhysicsManager;
	btRigidBody* mBody;

	PositionComponent* mPositionComponent;
};

#endif // physicsComponent_h__
