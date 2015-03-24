#include "pch.h"
#include "eventSystem.h"

// Global Event queue
static std::queue<Event*> mEventList;

// Pooled Inactive Events
static std::vector<Event*> mEventPool;

// Event Listeners
static std::list<std::weak_ptr<EventListener>> mEventListeners;

// Makes system safer when adding and removing listeners
static std::queue<std::weak_ptr<EventListener>> mNewEventListeners;
static std::vector<std::weak_ptr<EventListener>> mRemovedListeners;

void eventsys::init( void )
{
	/* Create a pool of events. */
	for( int i=0; i<MAX_EVENT_POOL; i++ )
	{
		mEventPool.push_back( new Event(EVT_NULL, EV_NULL));
	}
}

/* An event given to this function must be added to a list which will then dispatch it to event listeners*/
void eventsys::dispatch( Event* e )
{

	/* Prevent Null Events */
	assert( e->getEventID() != EV_NULL );

	// Push to a queue so we don't invalid iterators when event handlers dispatch events themselves
	mEventList.push(e);
		
}

void eventsys::dispatchNow(Event* e)
{

	for( auto i = mEventListeners.begin(); i != mEventListeners.end(); i++ )
	{
		std::shared_ptr<EventListener> listener = (*i).lock();

		if( listener )
		{
			// Don't dispatch event back to it dispatcher
			if( listener.get() != e->sentBy )
				listener->handle(e);
		}

	}

	eventsys::release(e);
}


/* Dispatch events to global listeners */
void eventsys::update( void )
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
			std::shared_ptr<EventListener> listener = (*i).lock();

			if( listener )
			{
				// Don't dispatch event back to it dispatcher
				if( listener.get() != e->sentBy )
					listener->handle(e);
			}

		}

		mEventList.pop();
		eventsys::release(e);
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
void eventsys::registerListener( std::weak_ptr<EventListener> e )
{
	mNewEventListeners.push(e);
}

/* Remove an event listener */
void eventsys::deregisterListener( std::weak_ptr<EventListener> e )
{
	mRemovedListeners.push_back(e);
}

/* Get a new event. This must remove it from the pool of events */
Event* eventsys::get( EVT_EventType eventType, EV_EventID eventID, int ID, EventListener* sentBy )
{
	Event* e;
	/* Check there are enough events, remove it, set type and return */
	if( mEventPool.size() > 0 )
	{
		e = mEventPool.back();
		mEventPool.pop_back();
		e->changeEventType(eventType);
		e->changeEventID(eventID);
	}
	else
	{
		/* Not enough events in pool, create a new one. This is slower - make sure there are enough in the pool to satisfy the game */
		e  = new Event(eventType,eventID);
		
	}

	/* Add ID */
	e->ID = ID;

	/* Add a sent by field to the event*/
	e->sentBy = sentBy;
	return e;
}

/* This should add the event back to the pool of events */
void eventsys::release(Event* e)
{
#ifdef _DEBUG
	e->d_initialised = false;
#endif
	e->changeEventType( EVT_NULL );
	e->changeEventID(EV_NULL); // Help identify rogue events

	/* Prevent duplicates in the pool */
	//if( std::find( mEventPool.begin(), mEventPool.end(), e ) == mEventPool.end() )
	//{
		mEventPool.push_back(e);
	//}
	//else
	//	OgreConsole::getSingletonPtr()->print("duplicate");
}


