//
//  eventSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//
#include "pch.h"
#include "eventSystem.h"
#include "engine/core/graphics/UI/ogreConsole.h"

template<> EventSystem* Ogre::Singleton<EventSystem>::msSingleton = 0;

EventSystem::EventSystem()
{
	d_eventDrift = 0;
}

void EventSystem::dispatchEvent( Event* e )
{
	// Make sure event isn't already on the list (TODO Fix why this is happening to begin with!!!)
	for( auto i=mEventList.begin(); i!=mEventList.end(); i++ )
		if( (*i) == e )
			return;

	// Add to active list
	mEventList.push_back(e);

}

void EventSystem::handleEvents()
{

	// Add new event listeners to list
	while( mNewEventListeners.size() > 0 )
	{
		mEventListeners.push_back( mNewEventListeners.front() );
		mNewEventListeners.pop();
	}
	if( mEventList.size() > 1 )
		OgreConsole::getSingletonPtr()->print(std::to_string(mEventList.size()));

    // loop through events
    while ( mEventList.size() > 0 ) {
        Event* e = mEventList.front();
		if( e->getEventType() == EV_CORE_MOUSE_MOVEMENT )
		{
			OgreConsole::getSingletonPtr()->print("MOUSEMOVE");

		}
		auto iter = mEventListeners.begin();

        // Dispatch events to listeners
		while ( iter != mEventListeners.end() )
		{

			bool toBeDeleted = false;

			// Check if iter is on the list to be removed
			for( auto i=mRemovedListeners.begin(); i!=mRemovedListeners.end();i++)
				if( (*i) == (*iter) )
				{
					mRemovedListeners.erase(i);
					toBeDeleted = true;
					break;
				}

			// TODO I'm not sure if this will work when removing multiple items
			// Test it at some point, for the time being it should work
			if( toBeDeleted == false )
			{
				(*iter)->handle(e);
				iter++;

				
			}else{
				iter=mEventListeners.erase(iter);
			}
		}


        // Put the event back in the inactive pool
        mEventList.pop_front();
        releaseEvent(e);
    }

}

void EventSystem::registerListener(EventListener* e)
{
    mNewEventListeners.push(e);
}

void EventSystem::deregisterListener(EventListener* e )
{
	mRemovedListeners.push_back(e);
}

Event* EventSystem::getEvent( int eventType )
{
	if( d_eventDrift != 0 )
	{
		OgreConsole::getSingletonPtr()->print( "Event Discrepency: " + std::to_string(d_eventDrift) );
	}

	d_eventDrift++;

	// See if there are any events pooled. If there are, return it. If there are not, create a new one, add it to the inactive pool, and return a pointer.
	if( mEventPool.size() > 0 )
	{
		Event* e = mEventPool.back();
		mEventPool.pop_back();
		e->changeEventType(eventType);
		e->mGUID = 0;
		return e;
	}
	else
	{
		Event* e = new Event( eventType );
		return e;
	}
}

void EventSystem::releaseEvent(Event* e)
{
	// Add it to the inactive pool.
	mEventPool.push_back(e);

	d_eventDrift--;

	
}
