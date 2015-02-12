#include "pch.h"
#include "physicsManager.h"

#define MOVE_TOLERANCE 0.00001

PhysicsManager::PhysicsManager( )
{
		
}

PhysicsManager::~PhysicsManager()
{

}

void PhysicsManager::initComponent( PhysicsComponent* comp, btCollisionShape* shape, btScalar mass )
{
	/* Physics Setup */
	btVector3 nullVector(0,0,0);
	shape->calculateLocalInertia( mass, nullVector);

	// Get the transform component of this entity **REQUIRED** 
	assert( entitysys::getByID(comp->ID)->hasComponent<TransformComponent>() );
	TransformComponent* trans = entitysys::getByID(comp->ID)->getComponent<TransformComponent>();

	/* Construct in correct position */
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass, new btDefaultMotionState( btTransform(trans->orientation,trans->position) ) , shape, nullVector);
	comp->body = new btRigidBody(fallRigidBodyCI);
	physics::addRigidBody(comp->body);


	comp->body->setWorldTransform(btTransform(trans->orientation,trans->position));
	comp->body->activate(true);

	comp->mass = mass;
	comp->shape = shape;
}

void PhysicsManager::deinitComponent(Component* comp)
{
	// Cast component to PhysicsComponent
	PhysicsComponent* phy = static_cast<PhysicsComponent*>(comp);
	physics::removeRigidBody(phy->body);
}


void PhysicsManager::update( double dt )
{
	/*	Process all of the transform and motion components and apply changes from the physics engine to them */
	for( auto i=mComponents.begin(); i != mComponents.end(); i++ )
	{
		/* Get physics component */
		PhysicsComponent* comp = dynamic_cast<PhysicsComponent*>(i->second);
		assert(comp!=nullptr);

		/* Get transform component */
		assert( entitysys::getByID(comp->ID)->hasComponent<TransformComponent>() ); // Must have position component
		TransformComponent* trans = entitysys::getByID(comp->ID)->getComponent<TransformComponent>();

		/* Update position */
		trans->position = comp->body->getWorldTransform().getOrigin();
		trans->orientation =  comp->body->getWorldTransform().getRotation();

		/* If the entity has a motionComponent, update that too */
		if( entitysys::getByID(comp->ID)->hasComponent<MotionComponent>() )
		{
			/* Get the MotionComponent*/
			MotionComponent* motion = entitysys::getByID(comp->ID)->getComponent<MotionComponent>();

			/* Velocities for dead reckoning on networked games */
			motion->velocity = comp->body->getLinearVelocity();
			motion->angularVelocity = comp->body->getAngularVelocity();
		}


	}
}

void PhysicsManager::handle( Event* e )
{
	switch ( e->getEventType() )
	{
	case EV_CORE_SET_TRANSFORM:

		// Get the component the event refers to
		if( componentExists( e->ID ) )
		{
			PhysicsComponent* phys = getComponentByID<PhysicsComponent>(e->ID);

			// Get the TransfromEvent
			TransformEvent* trans = e->getData<TransformEvent>();

			// Update internals.
			phys->body->setWorldTransform( btTransform( trans->orientation, trans->position ) );		
		}
		break;

	case EV_CORE_SET_MOTION:
		// Get the component the event refers to
		if( componentExists( e->ID ) )
		{
			PhysicsComponent* phys = getComponentByID<PhysicsComponent>(e->ID);

			// Get the MotionEvent
			MotionEvent* mot = e->getData<MotionEvent>();

			// Update internals.
			phys->body->setLinearVelocity( mot->velocity );
			phys->body->setAngularVelocity( mot->angularVelocity );
		}
		break;

	}
}

