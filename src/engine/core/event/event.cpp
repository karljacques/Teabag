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
	GUID = 0;
	ID = 0;
	sentBy = nullptr;
}

int Event::getEventType()
{
    return mEventType;
}


void Event::changeEventType(int ev)
{
	mEventType = ev;
}

void Event::clone( Event* e )
{
	// Only to be used in conjunction with EventSystem.
	// EventType and sentBy to be set by EventSystem
	this->GUID = e->GUID;
	this->ID = e->ID;

	memcpy( &this->data[0], &e->data[0], EVENT_PAYLOAD_SIZE );

#ifdef _DEBUG
	this->d_initialised = e->d_initialised;
#endif
}

char* Event::getRawData()
{
	return data;
}




