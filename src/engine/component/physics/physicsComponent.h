#ifndef physicsComponent_h__
#define physicsComponent_h__

#include "common.h"

#include "engine/component/component.h"

#include "engine/core/physicsSystem.h"
#include "engine/core/entitySystem.h"
#include "engine/core/eventSystem.h"

#include "engine/component/physics/transformComponent.h"

class PhysicsComponent :public Component
{
public:
	PhysicsComponent( ){

	};
	~PhysicsComponent(){};

	btRigidBody* body;
	btCollisionShape* shape;
	btScalar mass;


	Component* clone( EntID ID )
	{
		btVector3 nullvector;
		shape->calculateLocalInertia( mass, nullvector );

		PhysicsComponent* comp = entitysys::createComponent<PhysicsComponent>(ID);

		// Clone the body, which belongs to bullet.
		btTransform lTransform( Quat(0,0,0,1), float3(0,0,0));

		if( entitysys::getByID(ID)->hasComponent<TransformComponent>() )
		{
			// Get transform component
			TransformComponent* trans =  entitysys::getByID(ID)->getComponent<TransformComponent>();
			lTransform = btTransform( trans->orientation, trans->position );
		}
		else
		{
			assert("Can a physics component exist without a positional component? I don't think so.");
		}

		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass, new btDefaultMotionState( lTransform ) , shape, nullvector);
		comp->body = new btRigidBody( fallRigidBodyCI );

		physics::addRigidBody(comp->body);
		
		comp->body->setGravity( body->getGravity() );
		comp->body->setDamping( body->getLinearDamping(), body->getAngularDamping() );
		comp->body->setWorldTransform(lTransform);

		comp->mass = mass;
		comp->shape = shape;

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
