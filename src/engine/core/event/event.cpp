//
//  event.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//

#include "event.h"

Event::Event( int eventType )
{
    mEventType = eventType;
}

int Event::getEventType()
{
    return mEventType;
}

MouseEvent::MouseEvent( int eventType ):Event(eventType)
{
 
}

KeyboardEvent::KeyboardEvent( int eventType ):Event(eventType)
{
    
}