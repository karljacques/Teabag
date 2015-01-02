//
//  event.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//
#include "pch.h"
#include "event.h"

Event::Event( int eventType )
{
    mEventType = eventType;
	mGUID = 0;
}

int Event::getEventType()
{
    return mEventType;
}


void Event::changeEventType(int ev)
{
	mEventType = ev;
}





