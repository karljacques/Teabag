//
//  eventSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//

#include "eventSystem.h"

EventSystem::EventSystem()
{

}

void EventSystem::dispatchEvent( Event* e )
{
    mEventList.push(e);
}

void EventSystem::handleEvents()
{

	// Add new event listeners to list
	while( mNewEventListeners.size() > 0 )
	{
		mEventListeners.push_back( mNewEventListeners.front() );
		mNewEventListeners.pop();
	}

    // loop through events
    while ( mEventList.size() > 0 ) {
        Event* e = mEventList.front();

		auto iter = mEventListeners.begin();

        // Dispatch events to listeners
		while ( iter != mEventListeners.end() )
		{
			// See if it should be removed
			// Literally feeling like a genius right now
			// Before calling handle on the eventlistener, see if it's in the list of listeners
			// waiting to be deleted. If it is, remove it, set the iterator to the next item
			while( mRemovedListeners.size() > 0 )
			{
				EventListener* e = mRemovedListeners.front();
				for (auto i = mEventListeners.begin(); i != mEventListeners.end(); i++ ) {

					if( e == (*i) )
					{
						// Erase sets the iterator at the next item
						iter = mEventListeners.erase(i);
						break;
					}

					mRemovedListeners.pop();

				}
			}

			// TODO I'm not sure if this will work when removing multiple items
			// Test it at some point, for the time being it should work
			(*iter)->handle(e);
			iter++;
		}


        // Delete the event
        mEventList.pop();
        delete e;
    }

}

void EventSystem::registerListener(EventListener* e)
{
    mNewEventListeners.push(e);
}

void EventSystem::deregisterListener(EventListener* e )
{
	mRemovedListeners.push(e);
}
