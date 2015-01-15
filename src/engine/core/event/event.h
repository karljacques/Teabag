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

#include "events/keyboardEvent.h"
#include "events/mouseEvent.h"
#include "events/transformEvent.h"
#include "events/messageEvent.h"
#include "eventListener.h"

#define EVENT_PAYLOAD_SIZE sizeof(TransformEvent) // Largest Event

typedef unsigned int EntID;

enum EV_EventType
{
	// DEFAULT
	EV_NULL,

	// Input
    EV_CORE_MOUSE_PRESS,
    EV_CORE_MOUSE_RELEASE,
    EV_CORE_KEY_PRESS,
    EV_CORE_KEY_RELEASE,
	EV_CORE_MOUSE_MOVEMENT,
	EV_CORE_TEXT_INPUT,
	EV_CORE_CHAT_MESSAGE,

	// Physics
	EV_CORE_TRANSFORM_UPDATE,
	EV_CORE_TRANSFORM_UPDATE_ORIENTATION,

	// 
	EV_CLIENT_WORLD_CREATE_STATIC_BOX,
	EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX,

	// NETWORK
	EV_NETWORK_TRANSFORM_UPDATE,
	EV_NETWORK_NONEVENT_SNAPSHOT
};

class EventListener;

class Event
{

public:

    Event( int eventType );

	// Holds the event type (EV_EventType) of this event.
    int     getEventType();

	// Changes the event type - only use this if you know what you're doing ;-)
	// Should always be copied first
	void changeEventType( int ev );

	// Setting and getting the GUID. This will only ever be done by the networking component and the rest of the components
	// should be fairly (or hopefully completely) agnostic about it. For example, physics component will dispatch and event, 
	// networkComponent will pick up on this event, attach the object's GUID to the event before pumping it up to the network system.
	// The reverse is also true, when an event is coming in to create an object, the new network component will be assigned
	// based on this GUID.
	EntID GUID;

	EntID ID;

	template <class T>
	T* getData();

	template <class T>
	T* createEventData();

	EventListener* sentBy;


private:

    int     mEventType;
	char data[EVENT_PAYLOAD_SIZE];
	
};

template <class T>
T* Event::getData()
{
	return reinterpret_cast<T*>(data);
}

template <class T>
T* Event::createEventData()
{
	assert( sizeof(T) <= EVENT_PAYLOAD_SIZE );
	T* ptr = new(data) T;
	return getData<T>();
}






#endif /* defined(__YorkshireTea__event__) */
