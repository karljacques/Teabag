#ifndef physicsComponent_h__
#define physicsComponent_h__

#include "component.h"
#include "engine/core/physics/physicsManager.h"

class PhysicsComponent :
	public Component
{
public:
	PhysicsComponent( PhysicsManager* physicsManager, Entity* ent );
	~PhysicsComponent(void);

	void setAsBox( float x, float y, float z );
	void update(  double dt );

private:
	PhysicsManager* mPhysicsManager;
	btRigidBody* mBody;
};

#endif // physicsComponent_h__
