//
//  event.h
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//

#ifndef __YorkshireTea__event__
#define __YorkshireTea__event__

#include <iostream>
#include <Math/float3.h>
#include <Math/Quat.h>

#include <RakNetTypes.h>

enum EV_EventType
{
	// Input
    EV_CORE_MOUSE_PRESS,
    EV_CORE_MOUSE_RELEASE,
    EV_CORE_KEY_PRESS,
    EV_CORE_KEY_RELEASE,
	EV_CORE_MOUSE_MOVEMENT,
	EV_CORE_TEXT_INPUT,

	// Physics
	EV_CORE_TRANSFORM_UPDATE,
	EV_CORE_APPLY_FORCE,

	// World
	EV_WORLD_CREATE_STATIC_BOX
};

class Event
{

public:

    Event( int eventType );

	// Holds the event type (EV_EventType) of this event.
    int     getEventType();

	// Setting and getting the GUID. This will only ever be done by the networking component and the rest of the components
	// should be fairly (or hopefully completely) agnostic about it. For example, physics component will dispatch and event, 
	// networkComponent will pick up on this event, attach the object's GUID to the event before pumping it up to the network system.
	// The reverse is also true, when an event is coming in to create an object, the new network component will be assigned
	// based on this GUID.
	RakNet::RakNetGUID mGUID;

private:

    int     mEventType;

};

#include "events/keyboardEvent.h"
#include "events/mouseEvent.h"
#include "events/transformEvent.h"





#endif /* defined(__YorkshireTea__event__) */
