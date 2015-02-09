#ifndef physicsComponent_h__
#define physicsComponent_h__

#include "common.h"

#include "engine/component/component.h"

#include "engine/core/physicsSystem.h"
#include "engine/core/entitySystem.h"
#include "engine/core/eventSystem.h"

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
		btVector3 nullvector;
		shape->calculateLocalInertia( mass, nullvector );

		PhysicsComponent* comp = entitysys::createComponent<PhysicsComponent>(ID);

		// Clone the body, which belongs to bullet.
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass, new btDefaultMotionState( btTransform(orientation,position) ) , shape, nullvector);
		comp->body = new btRigidBody( fallRigidBodyCI );

		physics::addRigidBody(comp->body);
		
		comp->body->setGravity( body->getGravity() );
		comp->body->setDamping( body->getLinearDamping(), body->getAngularDamping() );
		comp->body->setWorldTransform(btTransform(orientation,position));

		comp->mass = mass;
		comp->shape = shape;
		comp->position = position;
		comp->orientation = orientation;

		/* Send out event to tell new position to components */
		Event* e = eventsys::get(EV_CORE_TRANSFORM_UPDATE);
		e->ID = comp->ID;
		TransformEvent* te = e->createEventData<TransformEvent>();
		te->position = position;
		te->orientation = orientation;
		eventsys::dispatch(e);

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
