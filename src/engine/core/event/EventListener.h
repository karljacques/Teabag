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
#include "Event.h"

class EventListener
{
public:
    
    virtual void handle( Event* e ) = 0;
    
    void    setEventType( int eventType );
    int     getEventType( );
private:

    int m_EventType;
    
};
#endif /* defined(__YorkshireTea__EventListener__) */
