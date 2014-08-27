#ifndef physicsComponent_h__
#define physicsComponent_h__

#include "engine/core/component/base/component.h"
#include "engine/core/physics/physicsManager.h"
#include "common.h"

class PhysicsComponent :
	public Component
{
public:
	PhysicsComponent( PhysicsManager* physicsManager );
	~PhysicsComponent(void);

	void setAsBox( float x, float y, float z );
	void update(  double dt );

	void initialise( btCollisionShape* shape, btScalar mass, float3 position, Quat orientation = Quat(0,0,0,1) );

	// Getters
	btRigidBody* getBody(){ return mBody; };

private:
	PhysicsManager* mPhysicsManager;
	btRigidBody* mBody;
};

#endif // physicsComponent_h__
