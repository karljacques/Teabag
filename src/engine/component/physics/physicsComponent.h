#ifndef physicsComponent_h__
#define physicsComponent_h__

#include "engine/component/component.h"
#include "common.h"

class PhysicsComponent :public Component
{
public:
	PhysicsComponent( ){
		position = float3(0,0,0);
		orientation = Quat(0,0,0,1);
	};
	~PhysicsComponent(){};

	btRigidBody* body;
	btCollisionShape* shape;
	btScalar mass;

	float3 position;
	Quat orientation;

	Component* clone()
	{
		PhysicsComponent* comp = new PhysicsComponent();
		comp->position = position;
		comp->orientation = orientation;

		// Clone the body, which belongs to bullet.
		comp->body = new btRigidBody( mass, new btDefaultMotionState( btTransform(orientation,position) ), shape, float3(0,0,0) );
		comp->mass = mass;
		comp->shape = shape;
		comp->position = position;
		comp->orientation = orientation;

		return comp;
	}
};

#endif // physicsComponent_h__
