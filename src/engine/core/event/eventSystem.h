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

using namespace std;

class EventSystem : public Ogre::Singleton<EventSystem>
{
public:

    EventSystem();

	// Add an event to the queue
    void dispatchEvent( Event* e );

	// Loop through events and dispatch to handlers (occurs at end of loop)
    void handleEvents();

	// Event listeners
    void registerListener( weak_ptr<EventListener> e );
    void deregisterListener( weak_ptr<EventListener> e );

	// Get an inactive event
	Event* getEvent( int eventType, int ID = 0, EventListener* sentBy = nullptr );

	// Puts an event back in the inactive pool.
	void releaseEvent( Event* e );

	// Debugging methods
	int d_getEventsToBeProcessed();

private:

	// Global Events
    std::deque<Event*> mEventList;

	// Events to be processed next loop
	std::deque<Event*> mEventQueue;

	// Pooled Inactive Events
	std::vector<Event*> mEventPool;

	// Event Listeners
    std::list<weak_ptr<EventListener>> mEventListeners;

	// Makes system safer when adding and removing listeners
	std::queue<weak_ptr<EventListener>> mNewEventListeners;
	std::vector<weak_ptr<EventListener>> mRemovedListeners;

	// Debugging variables
	int d_eventDrift;
};
#endif /* defined(__YorkshireTea__eventSystem__) */
