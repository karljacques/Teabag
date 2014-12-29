#include "pch.h"
#include "engine/core/event/eventSystem.h"
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
	if( mPositionComponent->getPosition().DistanceSq( mBody->getWorldTransform().getOrigin() ) > 0.0025 
		|| mPositionComponent->getOrientation().AngleBetween( mBody->getWorldTransform().getRotation() ) > 0.0025 ) 
	{
		mPositionComponent->_setPosition( mBody->getWorldTransform().getOrigin() );
		mPositionComponent->_setOrientation( mBody->getWorldTransform().getRotation() );

		Event* e = EventSystem::getSingletonPtr()->getEvent( EV_CORE_TRANSFORM_UPDATE );
		TransformEvent* me = e->createEventData<TransformEvent>();

		me->mQuaternion = mPositionComponent->getOrientation();
		me->mFloat3_1 = mPositionComponent->getPosition();

		me->mFloat3_2 = mBody->getLinearVelocity();
		me->mFloat3_3= mBody->getAngularVelocity();
		

		dispatch(e);

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
