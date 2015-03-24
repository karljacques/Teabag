//
//  event.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//
#include "pch.h"
#include "event.h"

Event::Event( EVT_EventType eventType, EV_EventID eventID )
{
	mEventType = eventType;
    mEventID = eventID;

	eGUID = 0;
	ID = 0;
	sentBy = nullptr;
}

EV_EventID Event::getEventID()
{
    return mEventID;
}


void Event::changeEventID(EV_EventID ev)
{
	mEventID = ev;
}

EVT_EventType Event::getEventType()
{
	return mEventType;
}

void Event::changeEventType(EVT_EventType EVT)
{
	this->mEventType = EVT;
}

void Event::clone( Event* e )
{
	// Only to be used in conjunction with EventSystem.
	// EventType and sentBy to be set by EventSystem
	this->eGUID = e->eGUID;
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







