#include "pch.h"
#include "positionComponent.h"


PositionComponent::PositionComponent(void)
{
	mPosition = float3(0,0,0);
	mOrientation = Quat(0,0,0,1);
}


PositionComponent::~PositionComponent(void)
{
}

void PositionComponent::update( double dt )
{
	if( mUpdated )
	{
		TransformEvent* m = new TransformEvent( EV_CORE_TRANSFORM_UPDATE );
		m->mFloat3_1 = mPosition;
		m->mQuaternion = mOrientation;

		dispatch( m );
		mUpdated = 0;
	}
}
void PositionComponent::handle( Event* e )
{
	if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
	{
		this->setPosition( static_cast<TransformEvent*>(e)->mFloat3_1);
		this->setOrientation( static_cast<TransformEvent*>(e)->mQuaternion);
	}

	// Interpolate between client position and host position
	if( e->getEventType() == EV_NETWORK_TRANSFORM_UPDATE )
	{
		TransformEvent* te = static_cast<TransformEvent*>(e);
		float3 newPosition = (this->mPosition + te->mFloat3_1)/2;
		
		this->setPosition( newPosition );
		this->setOrientation(te->mQuaternion);
	}
}

void PositionComponent::setPosition( float3 pos )
{
	mPosition = pos;

	TransformEvent* me = new TransformEvent( EV_CORE_TRANSFORM_UPDATE );
	me->mFloat3_1 = pos;
	me->mQuaternion = mOrientation; // For the sake of the physics engine
	dispatch( me );
}

float3 PositionComponent::getPosition()
{
	return mPosition;
}

void PositionComponent::setOrientation( Quat orientation )
{
	mOrientation = orientation;

	TransformEvent* me = new TransformEvent( EV_CORE_TRANSFORM_UPDATE );
	me->mQuaternion = orientation;
	me->mFloat3_1 = mPosition; // For the sake of the physics engine
	dispatch( me );
}

Quat PositionComponent::getOrientation()
{
	return mOrientation;
}

void PositionComponent::_setPosition( float3 pos )
{
	mPosition = pos;
}

void PositionComponent::_setOrientation( Quat rot )
{
	mOrientation = rot;
}