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

enum EV_EventType
{
	// Input
    EV_CORE_MOUSE_MOVE,
    EV_CORE_MOUSE_PRESS,
    EV_CORE_MOUSE_RELEASE,
    EV_CORE_KEY_PRESS,
    EV_CORE_KEY_RELEASE,
	EV_CORE_MOUSE_MOVEMENT,

	// Physics
	EV_CORE_TRANSFORM_UPDATE,
	EV_CORE_APPLY_FORCE
};

class Event
{

public:

    Event( int eventType );

    int     getEventType();

private:

    int     mEventType;

};

#include "events/keyboardEvent.h"
#include "events/mouseEvent.h"
#include "events/transformEvent.h"





#endif /* defined(__YorkshireTea__event__) */
