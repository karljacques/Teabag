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
		m->mPosition = mPosition;
		m->mOrientation = mOrientation;

		dispatch( m );
		mUpdated = 0;
	}
}
void PositionComponent::handle( Event* e )
{

}

void PositionComponent::setPosition( float3 pos )
{
	mPosition = pos;

	TransformEvent* me = new TransformEvent( EV_CORE_TRANSFORM_UPDATE );
	me->mPosition = pos;
	me->mOrientation = mOrientation; // For the sake of the physics engine
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
	me->mOrientation = orientation;
	me->mPosition = mPosition; // For the sake of the physics engine
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