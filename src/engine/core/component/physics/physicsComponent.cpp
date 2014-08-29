#include "pch.h"
#include "physicsComponent.h"
#include "engine/core/component/base/entity.h"

PhysicsComponent::PhysicsComponent( PhysicsManager* physicsManager, PositionComponent* positionComponent )
{
	mPhysicsManager = physicsManager;
	mPositionComponent = positionComponent;
}


PhysicsComponent::~PhysicsComponent(void)
{
}

void PhysicsComponent::setAsBox( float x, float y, float z )
{
	btCollisionShape* boxShape = new  btBoxShape( btVector3( x,y,z ));
	initialise(boxShape, 1, float3(0,30,0) );
}

void PhysicsComponent::update(  double dt  )
{
	mPositionComponent->_setPosition( mBody->getWorldTransform().getOrigin() );
	mPositionComponent->_setOrientation( mBody->getWorldTransform().getRotation() );

	MovementEvent* me = new MovementEvent( EV_MOVEMENT );
	me->mOrientation = mPositionComponent->getOrientation();
	me->mPosition = mPositionComponent->getPosition();

	dispatch(me);
}

void PhysicsComponent::initialise( btCollisionShape* shape, btScalar mass, float3 position, Quat orientation )
{
	btVector3 nullVector(0,0,0);
	shape->calculateLocalInertia( 1, nullVector);

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( mass,
		new btDefaultMotionState( btTransform( orientation, position))
		, shape, nullVector);

	mBody = new btRigidBody(fallRigidBodyCI);
	mPhysicsManager->getDiscreteDynamicsWorld()->addRigidBody(mBody);
}

void PhysicsComponent::handle( Event* e )
{
	switch( e->getEventType() )
	{
	case EV_MOVEMENT:
		MovementEvent* me = static_cast<MovementEvent*>(e);
		btTransform trans( me->mOrientation, me->mPosition );
		mBody->setWorldTransform( trans );
		mBody->activate(true);
		break;

	}
}
