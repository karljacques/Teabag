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
    m_EventList.push(e);
}

void EventSystem::handleEvents()
{
    // loop through events
    while ( m_EventList.size() > 0 ) {
        Event* e = m_EventList.front();
        
        // Dispatch events to listeners
        for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); i++ )
        {
            if( (*i)->getEventType() && e->getEventType() )
                (*i)->handle(e);
        }
        
        // Delete the event
        m_EventList.pop();
        delete e;
    }
}

void EventSystem::registerListener(EventListener* e)
{
    m_EventListeners.push_back(e);
}

void EventSystem::deregisterListener(EventListener* e )
{
    for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); i++ ) {
        
        if( e == (*i) )
        {
            m_EventListeners.erase(i);
            break;
        }
            
    }
}