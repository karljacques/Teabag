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
