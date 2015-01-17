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
	char* data_ptr = e->getRawData();

	memcpy( this->data, &data_ptr, EVENT_PAYLOAD_SIZE );
}

char* Event::getRawData()
{
	return data;
}




