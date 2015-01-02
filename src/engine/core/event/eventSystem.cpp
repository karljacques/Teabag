//
//  eventSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//
#include "pch.h"
#include "eventSystem.h"



template<> EventSystem* Ogre::Singleton<EventSystem>::msSingleton = 0;

EventSystem::EventSystem()
{
	/* Create a pool of events. */
	for( int i=0; i<MAX_EVENT_POOL; i++ )
	{
		mEventPool.push_back(new Event(EV_NULL));
	}
}

/* An event given to this function must be added to a list which will then dispatch it to event listeners*/
void EventSystem::dispatchEvent( Event* e )
{
	/* Prevent Null Events */
	if( e->getEventType() == EV_NULL )
		return;

	/* Prevent duplicates */
	if( std::find( mEventList.begin(), mEventList.end(), e ) == mEventList.end() && std::find( mEventQueue.begin(), mEventQueue.end(),e ) == mEventQueue.end() )
	{
		// Push to a queue so we don't invalid iterators when event handlers dispatch events themselves
		mEventQueue.push_back(e);
	}else
	{
		OgreConsole::getSingletonPtr()->print("Duplicate Event");
		releaseEvent(e);
		
	}
}

/* Dispatch events to global listeners */
void EventSystem::handleEvents()
{
	/* Add any pending events */
	while( mEventQueue.size() > 0 )
	{
		mEventList.push_back( mEventQueue.front() );
		mEventQueue.pop_front();
	}

	/* Add any pending listeners */
	while( mNewEventListeners.size() > 0 )
	{
		mEventListeners.push_back( mNewEventListeners.front() );
		mNewEventListeners.pop();
	}

	/* Loop through all listeners, first check if the listener has been added to the list of listeners to be removed
	    It's possible that the listener could already have been deleted and we will crash if we dereference it */
	for( auto lsnr = mEventListeners.begin(); lsnr != mEventListeners.end(); lsnr++ )
	{
		bool iterValid = true;
		for( auto del = mRemovedListeners.begin(); del != mRemovedListeners.end(); del++ )
		{
			/* Check if they point to the same thing */
			if( (*del) == (*lsnr ) )
			{
				/* don't try to dereference the listener */
				iterValid = false;
			}
		}

		/* Do a check to see if the iterator is valid or not. 
		  If it is, loop through the events and dispatch them to it.
		  If it is not, do nothing, iterator will be increased and the listener will be removed at a later point */
		if( iterValid )
		{
			for( auto e = mEventList.begin(); e != mEventList.end(); e++ )
			{
				(*lsnr)->handle(*e);			
			}
		}
	}

	/* All done - clean up time.
		Remove deleted listeners */
	for( auto del = mRemovedListeners.begin(); del != mRemovedListeners.end(); del++ )
	{
		for( auto lsnr = mEventListeners.begin(); lsnr != mEventListeners.end(); lsnr++ )
		{
			if( (*del) == (*lsnr ))
				mEventListeners.erase(lsnr);
		}
	}
	mRemovedListeners.clear();

	/* Release all events back to event pool */
	for( auto e = mEventList.begin(); e != mEventList.end(); e++ )
	{
		releaseEvent(*e);
	}
	mEventList.clear();

}

/*	Add a new event listener.
	It is the user's responsibility to check that there are no duplicates */
void EventSystem::registerListener(EventListener* e)
{
	mNewEventListeners.push(e);
}

/* Remove an event listener */
void EventSystem::deregisterListener(EventListener* e )
{
	mRemovedListeners.push_back(e);
}

/* Get a new event. This must remove it from the pool of events */
Event* EventSystem::getEvent( int eventType )
{
	/* Check there are enough events, remove it, set type and return */
	if( mEventPool.size() > 0 )
	{
		Event* e = mEventPool.back();
		mEventPool.pop_back();
		e->changeEventType(eventType);
		return e;
	}
	else
	{
		/* Not enough events in pool, create a new one. This is slower - make sure there are enough in the pool to satisfy the game */
		OgreConsole::getSingletonPtr()->print("Insufficient Events - created a new one");
		return new Event(eventType);
	}
}

/* This should add the event back to the pool of events */
void EventSystem::releaseEvent(Event* e)
{
	e->changeEventType(EV_NULL); // Help identify rogue events

	/* Prevent duplicates in the pool */
	if( std::find( mEventPool.begin(), mEventPool.end(), e ) == mEventPool.end() )
	{
		mEventPool.push_back(e);
	}
}
