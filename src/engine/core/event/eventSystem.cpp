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
	mRemovedListeners.push_back(e);
}
