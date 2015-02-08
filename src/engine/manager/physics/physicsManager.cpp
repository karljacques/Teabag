#include "pch.h"
#include "physicsManager.h"
#include "../../core/event/eventSystem.h"
#include "../../core/physics/physicsSystem.h"

#define MOVE_TOLERANCE 0.00001

PhysicsManager::PhysicsManager( )
{
		
}

PhysicsManager::~PhysicsManager()
{

}

void PhysicsManager::initComponent( PhysicsComponent* comp, btCollisionShape* shape, btScalar mass, float3 pos, Quat rot)
{
	/* Physics Setup */
	btVector3 nullVector(0,0,0);
	shape->calculateLocalInertia( mass, nullVector);

	/* Construct in correct position */
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass, new btDefaultMotionState( btTransform(rot,pos) ) , shape, nullVector);
	comp->body = new btRigidBody(fallRigidBodyCI);
	physics::addRigidBody(comp->body);

	/* Send out event to tell new position to components */
	Event* e = eventsys::get(EV_CORE_TRANSFORM_UPDATE);
	e->ID = comp->ID;
	TransformEvent* te = e->createEventData<TransformEvent>();
	te->position = pos;
	te->orientation = rot;
	eventsys::dispatch(e);

	comp->position = pos;
	comp->orientation = rot;

	comp->body->setWorldTransform(btTransform(rot,pos));
	comp->body->activate(true);

}

void PhysicsManager::update( double dt )
{
	/*	Process all of the positional components and check if they have moved - if so send out position updates.
		This will also be where you send out events for collisions etc */
	for( auto i=mComponents.begin(); i != mComponents.end(); i++ )
	{
		PhysicsComponent* comp = dynamic_cast<PhysicsComponent*>(i->second);
		assert(comp!=nullptr);
		/* If the object has moved past the move tolerance since the last update, send out events and update the position data */
		if( comp->position.DistanceSq( comp->body->getWorldTransform().getOrigin() ) > MOVE_TOLERANCE 
			|| comp->orientation.AngleBetween( comp->body->getWorldTransform().getRotation() ) > MOVE_TOLERANCE )
		{
			/* Update position */
			comp->position = comp->body->getWorldTransform().getOrigin();
			comp->orientation =  comp->body->getWorldTransform().getRotation();

			/* Create event */
			Event* e = eventsys::get( EV_CORE_TRANSFORM_UPDATE,comp->ID,this );
			TransformEvent* me = e->createEventData<TransformEvent>();

			me->orientation =comp->orientation;
			me->position = comp->position;

			/* Velocities for dead reckoning on networked games */
			me->velocity = comp->body->getLinearVelocity();
			me->angularVelocity = comp->body->getAngularVelocity();

			eventsys::dispatch(e);
		}
	}
}

void PhysicsManager::handle( Event* e )
{
	if( componentExists(e->ID) )
	{
		PhysicsComponent* comp = getComponentByID<PhysicsComponent>( e->ID );
		switch( e->getEventType() )
		{


		case EV_CORE_TRANSFORM_UPDATE:
			{
				TransformEvent* me = e->getData<TransformEvent>();
				btTransform trans( me->orientation, me->position );
				comp->body->setWorldTransform( trans );
				comp->body->activate(true);

				
				break;
			}
			/*
		case EV_CORE_TRANSFORM_UPDATE_ORIENTATION:
			{
				TransformEvent* me = e->getData<TransformEvent>();
				btTransform trans(me->orientation, comp->position );
				comp->body->setWorldTransform( trans );
				comp->body->activate(true);
				break;
			}
			*/

		case EV_NETWORK_TRANSFORM_UPDATE:
			{
				TransformEvent* me = e->getData<TransformEvent>();
				btTransform trans( me->orientation, me->position );

				comp->body->setWorldTransform( trans );
				comp->body->activate(true);
				break;
			}
		}
	}
}
