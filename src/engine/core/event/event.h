#ifndef __YorkshireTea__event__
#define __YorkshireTea__event__

#include <iostream>
#include <Math/float3.h>
#include <Math/Quat.h>

#include "eventListener.h"
#include "common.h"

#define EVENT_PAYLOAD_SIZE 128 // Largest Event

// All the possible types of event used within the game.
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
	EV_CORE_PRINT_CONSOLE,

	// Physics
	EV_CORE_SET_TRANSFORM,
	EV_CORE_SET_MOTION,

	// TEMPORARY GAME MODIFIERS
	EV_CLIENT_WORLD_CREATE_STATIC_BOX,
	EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX,

	// NETWORK
	EV_NETWORK_INCOMING_SNAPSHOT,
	EV_NETWORK_NEW_CONNECTION,

	// NETWORK - PLAYER MODIFIERS
	EV_NETWORK_PLAYER_JOINED,
	EV_NETWORK_PLAYER_DATA,

	EV_NETWORK_PLAYER_LEFT,
	EV_NETWORK_PLAYER_DROPPED,
	EV_NETWORK_PLAYER_KICKED,

	// NETWORK - STATE MODIFIERS
	EV_NETWORK_INIT_READY,

	// NETWORK - MODE MODIFIERS
	EV_NETWORK_MOD_CLIENT,
	EV_NETWORK_MOD_CONNECT,

	// PLAYER MODIFERS
	EV_PLAYER_SET_NAME,

	// SPAWN MODIFIERS
	EV_SPAWN_SPECTATOR

};

class EventListener;

class Event
{

public:
					Event( int eventType );

	// Returns the event type (EV_EventType) of this event.
    int				getEventType();

	// Changes the event type - only use this if you know what you're doing ;-)
	// Should always be copied first
	void			changeEventType( int ev );

	/* pGUID represents the GUID of the client that sent the event - it will be attached by the networkSystem before sending.*/
	PlayerGUID		pGUID; 

	/* eGUID represents the GUID of the entity the event is relevant to - eGUID will be assigned by being attached
	indirectly by the networkSystem by calling _find_free_guid(), attaching it to an object creation event and then 
	sending the event across the network. Upon object creation at the receiving end, the entity will take the eGUID 
	from the event and attach it to the entity. 
	In all subsequent calls, that entity can be referred to by it's eGUID. */
	EntGUID		eGUID;

	/* Similar to eGUID except that this represents a local ID on this machine. This is useful where an entity may not
	necessarily be synced across a network. That being said, it will be more efficient to merge ID and eGUID into one, 
	and simply do not network sync the object if it does not have a networkComponent. 
	TODO */
	EntID			ID;

	/* Method to get the data stored within the event payload. It will automatically cast it to the type T.
			Usage: TypeOfEventPayload* data =  e->getData<TypeOfEventPayload>(); 
	This data must have already been created using createEventData - whether this be on the local machine or the remote machine. */
	template <class T>
	T*				getData();

	/* Method to create data in the event payload. Performs a placement new on a block of data and returns a pointer to
	the object created there. Data can be retrieved after creation using getData method. 
		Usage: TypeOfEventPayload* data = e->createEventData<TypeofEventPayload>();
	*/
	template <class T>
	T*				createEventData();

	/* EventListener that the event was sent by. In cases where the event listener will also handle the event sent, it
	is important to set sentBy to 'this' to prevent a nasty feedback loop. This can be done using an extra parameter in
	the EventSystem::getEvent method. */
	EventListener*	sentBy;

	/* In cases where you have to immediately new some event from data (i.e. when an event comes in over the network, this
	method should be used to copy data from a new'd or reinterpreted event into one that has previously been created by the EventSystem.
		Usage:	Event* tempEvent = reinterpret_cast<Event*>(data);
				Event* realEvent = EventSystem::getEvent();
				realEvent.clone(tempEvent);
				delete tempEvent;
	For obvious reasons, in real situations only delete tempEvent if you know it will no longer be needed. */
	void			clone( Event* e );

	/* Returns ptr to data. Used in clone method */
	char*			getRawData();

	/* Only exists in debug mode - performs check that event has been initialised before it can be used.
	i.e: createEventData is called *before* getdata */
#ifdef _DEBUG
	bool			d_initialised;
#endif

private:

	/* Type of the event. Can be set in the construct or changed using changeEventType. Retrieved with getEventType. */
    int				mEventType;

	/* This is the payload of the event, it can be casted to different classes using methods createEventData and getData */
	char			data[EVENT_PAYLOAD_SIZE];
	
};

template <class T>
T* Event::getData()
{
#ifdef _DEBUG
	// Perform check that data is initialised, assert if not.
	assert( d_initialised == true );
#endif
	// Cast data to given type
	return reinterpret_cast<T*>(data);
}

template <class T>
T* Event::createEventData()
{
	// Initialise event
#ifdef _DEBUG
	d_initialised = true;
#endif

	// Assert that the payload is big enough to hold that class of object.
	// Perform a placement new.
	assert( sizeof(T) <= EVENT_PAYLOAD_SIZE );
	T* ptr = new(data) T;

	return getData<T>();
}






#endif /* defined(__YorkshireTea__event__) */
