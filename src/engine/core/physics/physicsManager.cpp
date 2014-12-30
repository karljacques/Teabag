#include "pch.h"
#include "physicsManager.h"
#include "..\event\eventSystem.h"

#define MOVE_TOLERANCE 0.0025

PhysicsManager::PhysicsManager( EntityManager* ent )
{
		
		// Build the broadphase
		btBroadphaseInterface* broadphase = new btDbvtBroadphase();

		// Set up the collision configuration and dispatcher
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

		// The actual physics solver
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

		// The world.
		mWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		mWorld->setGravity(btVector3(0, -9.81f, 0));
}

PhysicsManager::~PhysicsManager()
{
	delete mWorld;
}

Component* PhysicsManager::createComponent( btCollisionShape* shape, btScalar* mass, float3 pos, Quat rot)
{
	/* Create Component*/
	PhysicsComponent* comp = new PhysicsComponent();
	mComponents.push_back( comp );

	/* Physics Setup */
	btVector3 nullVector(0,0,0);
	shape->calculateLocalInertia( 1, nullVector);

	/* Construct in correct position */
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass,
		new btDefaultMotionState( btTransform( rot, pos))
		, shape, nullVector);

	comp->body = new btRigidBody(fallRigidBodyCI);
	mWorld->addRigidBody(comp->body);

	return comp;
}

void PhysicsManager::update( double dt )
{
	mWorld->stepSimulation( 1.0/60.0f ,8 );

	/*	Process all of the positional components and check if they have moved - if so send out position updates.
		This will also be where you send out events for collisions etc */
	for( auto i=mComponents.begin(); i != mComponents.end(); i++ )
	{
		PhysicsComponent* comp = *i;

		/* If the object has moved past the move tolerance since the last update, send out events and update the position data */
		if( comp->position.DistanceSq( comp->body->getWorldTransform().getOrigin() ) > MOVE_TOLERANCE 
			|| comp->orientation.AngleBetween( comp->body->getWorldTransform().getRotation() ) > MOVE_TOLERANCE )
		{
			/* Update position */
			comp->position = comp->body->getWorldTransform().getOrigin();
			comp->orientation =  comp->body->getWorldTransform().getRotation();

			/* Create event */
			Event* e = EventSystem::getSingletonPtr()->getEvent( EV_CORE_TRANSFORM_UPDATE );
			TransformEvent* me = e->createEventData<TransformEvent>();

			me->mQuaternion =comp->orientation;
			me->mFloat3_1 = comp->position;

			/* Velocities for dead reckoning on networked games */
			me->mFloat3_2 = comp->body->getLinearVelocity();
			me->mFloat3_3= comp->body->getAngularVelocity();

			EventSystem::getSingletonPtr()->dispatchEvent(e);
		}
	}
}

void PhysicsManager::handle( Event* e )
{
	switch( e->getEventType() )
	{
	case EV_CORE_TRANSFORM_UPDATE:
		{
			TransformEvent* me = e->getData<TransformEvent>();
			btTransform trans( me->mQuaternion, me->mFloat3_1 );
			mBody->setWorldTransform( trans );
			mBody->activate(true);
			break;
		}

	case EV_CORE_APPLY_FORCE:
		{
			TransformEvent* te = e->getData<TransformEvent>();
			getBody()->applyCentralForce(  Quat(getBody()->getWorldTransform().getRotation()).Transform( te->mFloat3_1 ) );
			break;
		}

	case EV_NETWORK_TRANSFORM_UPDATE:
		{
			TransformEvent* me = e->getData<TransformEvent>();
			getBody()->setLinearVelocity( me->mFloat3_2 );
			getBody()->setAngularVelocity( me->mFloat3_3 );
			mBody->activate(true);
			break;
		}
	}
}
