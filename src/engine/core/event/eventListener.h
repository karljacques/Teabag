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

class EventListener
{
public:

    virtual void handle( Event* e ) = 0;

    void    setEventType( const int eventType );
    int     getEventType( );
private:

    int m_EventType;

};
#endif /* defined(__YorkshireTea__EventListener__) */
