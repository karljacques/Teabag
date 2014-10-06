#ifndef transformEvent_h__
#define transformEvent_h__

#include "../event.h"

// Can be used to update transformation or used to apply a force.
class TransformEvent : public Event
{
public:
	TransformEvent( int EV_EventType );

	float3 mPosition;
	Quat mOrientation;

	float3 mSecondaryFloat; // Used to dictate size, direction - when not used as a transform event

	unsigned int getSize();
};

#endif // transformEvent_h__
