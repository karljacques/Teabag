//
//  eventSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//
#include "pch.h"
#include "eventSystem.h"
#include "../user-interface/ogreConsole.h"

using namespace std;

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
	// Push to a queue so we don't invalid iterators when event handlers dispatch events themselves
	mEventList.push(e);
		
}

/* Dispatch events to global listeners */
void EventSystem::update()
{
	/* Add any pending listeners */
	while( mNewEventListeners.size() > 0 )
	{
		mEventListeners.push_back( mNewEventListeners.front() );
		mNewEventListeners.pop();
	}

	/* loop through events and loop through listeners, dispatch events to listeners*/
	while( mEventList.size() > 0 )
	{
		Event* e = mEventList.front();

		for( auto i = mEventListeners.begin(); i != mEventListeners.end(); i++ )
		{
			shared_ptr<EventListener> listener = (*i).lock();

			if( listener )
			{
				// Don't dispatch event back to it dispatcher
				if( listener.get() != e->sentBy )
					listener->handle(e);
			}

		}

		mEventList.pop();
		this->release(e);
	}

	/* All done - clean up time.
		Remove deleted listeners */
	while( mRemovedListeners.size() > 0)
	{
		std::weak_ptr<EventListener> del = mRemovedListeners.back();
		mEventListeners.remove_if( [del](std::weak_ptr<EventListener> p )
			{
				std::shared_ptr<EventListener> swp = del.lock();
				std::shared_ptr<EventListener> sp = p.lock();

				if( swp && sp )
					return swp == sp;
				return false;
			}); // Don't quite understand lambda syntax. Stolen from here: http://stackoverflow.com/questions/10120623/removing-item-from-list-of-weak-ptrs
		mRemovedListeners.pop_back();
	}

}

/*	Add a new event listener.
	It is the user's responsibility to check that there are no duplicates */
void EventSystem::registerListener( weak_ptr<EventListener> e )
{
	mNewEventListeners.push(e);
}

/* Remove an event listener */
void EventSystem::deregisterListener( weak_ptr<EventListener> e )
{
	mRemovedListeners.push_back(e);
}

/* Get a new event. This must remove it from the pool of events */
Event* EventSystem::getEvent( int eventType, int ID, EventListener* sentBy )
{
	Event* e;
	/* Check there are enough events, remove it, set type and return */
	if( mEventPool.size() > 0 )
	{
		e = mEventPool.back();
		mEventPool.pop_back();
		e->changeEventType(eventType);
	}
	else
	{
		/* Not enough events in pool, create a new one. This is slower - make sure there are enough in the pool to satisfy the game */
		e  = new Event(eventType);
		
	}

	/* Add ID */
	e->ID = ID;

	/* Add a sent by field to the event*/
	e->sentBy = sentBy;
	return e;
}

/* This should add the event back to the pool of events */
void EventSystem::release(Event* e)
{
	e->changeEventType(EV_NULL); // Help identify rogue events

	/* Prevent duplicates in the pool */
	//if( std::find( mEventPool.begin(), mEventPool.end(), e ) == mEventPool.end() )
	//{
		mEventPool.push_back(e);
	//}
	//else
	//	OgreConsole::getSingletonPtr()->print("duplicate");
}

