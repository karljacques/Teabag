//
//  EventListener.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//
#include "pch.h"
#include "eventListener.h"

EVT_EventType EventListener::getEventType()
{
    return m_EventType;
}

void EventListener::setEventType( EVT_EventType eventType )
{
    m_EventType = eventType;
}

EventListener::EventListener()
{
	m_EventType = EVT_NULL;
}

EventListener::~EventListener()
{

}
