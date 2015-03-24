//
//  EventListener.h
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//

#ifndef __YorkshireTea__EventListener__
#define __YorkshireTea__EventListener__

#include <iostream>
#include "event.h"

class Event;
enum EVT_EventType;

class EventListener
{
public:

	EventListener();
	virtual ~EventListener();

    virtual void handle( Event* e ) = 0;

    void			setEventType( EVT_EventType eventType );
    EVT_EventType   getEventType( );
private:

    EVT_EventType m_EventType;

};
#endif /* defined(__YorkshireTea__EventListener__) */
