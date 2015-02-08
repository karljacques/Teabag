#ifndef __YorkshireTea__eventSystem__
#define __YorkshireTea__eventSystem__

#include "pch.h"

#include "event.h"
#include "eventListener.h"

#define MAX_EVENT_POOL 150

namespace eventsys
{
	void init( void );

	// Add an event to the queue
	void dispatch( Event* e );

	// Loop through events and dispatch to handlers (occurs at end of loop)
	void update( void );

	// Event listeners
	void registerListener( std::weak_ptr<EventListener> e );
	void deregisterListener( std::weak_ptr<EventListener> e );

	// Get an inactive event
	Event* get( int eventType, int ID = 0, EventListener* sentBy = nullptr );

	// Puts an event back in the inactive pool.
	void release( Event* e );
}





#endif /* defined(__YorkshireTea__eventSystem__) */
