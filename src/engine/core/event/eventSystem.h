//
//  eventSystem.h
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//

#ifndef __YorkshireTea__eventSystem__
#define __YorkshireTea__eventSystem__

#include "pch.h"

#include "event.h"
#include "eventListener.h"


#define MAX_EVENT_POOL 150

void eventInit();

// Add an event to the queue
void eventDispatch( Event* e );

// Loop through events and dispatch to handlers (occurs at end of loop)
void eventUpdate( double dt );

// Event listeners
void eventRegisterListener( std::weak_ptr<EventListener> e );
void eventDeregisterListener( std::weak_ptr<EventListener> e );

// Get an inactive event
Event* eventGetPooled( int eventType, int ID = 0, EventListener* sentBy = nullptr );

// Puts an event back in the inactive pool.
void eventRelease( Event* e );




#endif /* defined(__YorkshireTea__eventSystem__) */
