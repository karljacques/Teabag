#include "pch.h"
#include "positionComponent.h"
#include "engine/core/event/eventSystem.h"

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
		Event* e = EventSystem::getSingletonPtr()->getEvent( EV_CORE_TRANSFORM_UPDATE );
		TransformEvent* m = e->createEventData<TransformEvent>();
		m->mFloat3_1 = mPosition;
		m->mQuaternion = mOrientation;

		dispatch( e );

		mUpdated = 0;
	}
}
void PositionComponent::handle( Event* e )
{
	if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
	{
		TransformEvent* te = e->getData<TransformEvent>();
		this->setPosition( te->mFloat3_1);
		this->setOrientation( te->mQuaternion);
	}

}

void PositionComponent::setPosition( float3 pos )
{
	mPosition = pos;

	Event* e = EventSystem::getSingletonPtr()->getEvent(EV_CORE_TRANSFORM_UPDATE);
	TransformEvent* me = e->createEventData<TransformEvent>();
	me->mFloat3_1 = pos;
	me->mQuaternion = mOrientation; // For the sake of the physics engine
	dispatch( e );

}

float3 PositionComponent::getPosition()
{
	return mPosition;
}

void PositionComponent::setOrientation( Quat orientation )
{
	mOrientation = orientation;

	Event* e = EventSystem::getSingletonPtr()->getEvent(EV_CORE_TRANSFORM_UPDATE);
	TransformEvent* me = e->createEventData<TransformEvent>();
	me->mQuaternion = orientation;
	me->mFloat3_1 = mPosition; // For the sake of the physics engine
	dispatch( e );
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