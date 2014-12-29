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

}

void EventSystem::dispatchEvent( Event* e )
{
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

    // loop through events
    while ( mEventList.size() > 0 ) {
        Event* e = mEventList.front();

		auto iter = mEventListeners.begin();
		if( e->getEventType() == EV_CORE_MOUSE_MOVEMENT )
		{
			OgreConsole::getSingletonPtr()->print("MouseMoveEvent");
		}

		if( e->getEventType() == EV_CORE_KEY_PRESS )
		{
			OgreConsole::getSingletonPtr()->print("KeyPressEvent");
		}

		if( e->getEventType() == EV_CORE_TEXT_INPUT )
		{
			OgreConsole::getSingletonPtr()->print("TextEvent");
		}
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

}
