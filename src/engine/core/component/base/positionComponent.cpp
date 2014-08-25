#include "pch.h"
#include "positionComponent.h"


PositionComponent::PositionComponent(void)
{
}


PositionComponent::~PositionComponent(void)
{
}

void PositionComponent::update( double dt )
{
	if( mUpdated )
	{
		MovementEvent* m = new MovementEvent( EV_Movement );
		m->mPosition = mPosition;
		m->mOrientation = mOrientation;

		dispatch( m );
		mUpdated = 0;
	}
}
void PositionComponent::handle( Event* e )
{
	if( e->getEventType() == EV_Movement )
	{
		MovementEvent* m = static_cast<MovementEvent*>(e);
		mPosition = m->mPosition;
		mOrientation = mOrientation;

		// Set events to be dispatched
		mUpdated = true;
	}
}