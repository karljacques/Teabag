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

class EventSystem : public Ogre::Singleton<EventSystem>
{
public:

    EventSystem();

	// Add an event to the queue
    void dispatchEvent( Event* e );

	// Loop through events and dispatch to handlers (occurs at end of loop)
    void handleEvents();

	// Event listeners
    void registerListener( EventListener* e );
    void deregisterListener( EventListener* e );

	// Get an inactive event
	Event* getEvent( int eventType );

	// Puts an event back in the inactive pool.
	void releaseEvent( Event* e );

private:

	// Global Events
    std::deque<Event*> mEventList;

	// Pooled Inactive Events
	std::vector<Event*> mEventPool;

	// Event Listeners
    std::vector<EventListener*> mEventListeners;

	// Makes system safer when adding and removing listeners
	std::queue<EventListener*> mNewEventListeners;
	std::vector<EventListener*> mRemovedListeners;

	// Debugging variables
	int d_eventDrift;
};
#endif /* defined(__YorkshireTea__eventSystem__) */
