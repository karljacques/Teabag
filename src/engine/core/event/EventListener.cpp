//
//  EventListener.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//

#include "EventListener.h"

int EventListener::getEventType()
{
    return m_EventType;
}

void EventListener::setEventType( int eventType )
{
    m_EventType = eventType;
}