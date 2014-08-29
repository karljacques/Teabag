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
		MovementEvent* m = new MovementEvent( EV_MOVEMENT );
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

	MovementEvent* me = new MovementEvent( EV_MOVEMENT );
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

	MovementEvent* me = new MovementEvent( EV_MOVEMENT );
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