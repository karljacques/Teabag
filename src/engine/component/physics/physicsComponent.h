#ifndef physicsComponent_h__
#define physicsComponent_h__

#include "common.h"

#include "engine/component/component.h"

#include "engine/core/physicsSystem.h"
#include "engine/core/entitySystem.h"

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

	Component* clone( EntID ID )
	{
		PhysicsComponent* comp = entitysys::createComponent<PhysicsComponent>(ID);
		comp->position = position;
		comp->orientation = orientation;

		// Clone the body, which belongs to bullet.
		comp->body = new btRigidBody( mass, new btDefaultMotionState( btTransform(orientation,position) ), shape, float3(0,0,0) );

		physics::addRigidBody(comp->body);
		
		comp->body->setGravity( body->getGravity() );
		comp->body->setDamping( body->getLinearDamping(), body->getAngularDamping() );

		comp->mass = mass;
		comp->shape = shape;
		comp->position = position;
		comp->orientation = orientation;

		return comp;
	}

	void activate()
	{
		body->activate(true);
	}

	void deactivate()
	{
		body->setActivationState(DISABLE_SIMULATION);
		physics::removeRigidBody(body);
	}
};

#endif // physicsComponent_h__
