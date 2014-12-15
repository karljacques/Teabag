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
	// Check to see if it has moved or rotated, if so inform the position component and send out events,.
	if( mPositionComponent->getPosition().DistanceSq( mBody->getWorldTransform().getOrigin() ) < 0.5 
		|| mPositionComponent->getOrientation().AngleBetween( mBody->getWorldTransform().getRotation() ) < 0.5 ) 
	{
		mPositionComponent->_setPosition( mBody->getWorldTransform().getOrigin() );
		mPositionComponent->_setOrientation( mBody->getWorldTransform().getRotation() );

		TransformEvent* me = new TransformEvent( EV_CORE_TRANSFORM_UPDATE );
		me->mOrientation = mPositionComponent->getOrientation();
		me->mPosition = mPositionComponent->getPosition();

		dispatch(me);
	}
	
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
	case EV_CORE_TRANSFORM_UPDATE:
		{
			TransformEvent* me = static_cast<TransformEvent*>(e);
			btTransform trans( me->mOrientation, me->mPosition );
			mBody->setWorldTransform( trans );
			mBody->activate(true);
			break;
		}

	case EV_CORE_APPLY_FORCE:
		{
			TransformEvent* te = static_cast<TransformEvent*>(e);
			getBody()->applyCentralForce(  Quat(getBody()->getWorldTransform().getRotation()).Transform( te->mPosition ) );
			break;
		}
	}
}
