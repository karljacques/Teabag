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
    // loop through events
    while ( mEventList.size() > 0 ) {
        Event* e = mEventList.front();

        // Dispatch events to listeners
        for (auto i = mEventListeners.begin(); i != mEventListeners.end(); i++ )
        {
            if( (*i)->getEventType() && e->getEventType() )
                (*i)->handle(e);
        }

        // Delete the event
        mEventList.pop();
        delete e;
    }
}

void EventSystem::registerListener(EventListener* e)
{
    mEventListeners.push_back(e);
}

void EventSystem::deregisterListener(EventListener* e )
{
    for (auto i = mEventListeners.begin(); i != mEventListeners.end(); i++ ) {

        if( e == (*i) )
        {
            mEventListeners.erase(i);
            break;
        }

    }
}
