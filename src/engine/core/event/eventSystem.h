//
//  eventSystem.h
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//

#ifndef __YorkshireTea__eventSystem__
#define __YorkshireTea__eventSystem__

#include <iostream>
#include <queue>
#include "event.h"
#include "EventListener.h"


class EventSystem
{
public:
    
    EventSystem();
    
    void dispatchEvent( Event* e );
    
    void handleEvents();
    
    void registerListener( EventListener* e );
    void deregisterListener( EventListener* e );
    
private:
    
    std::queue<Event*> m_EventList;
    std::deque<EventListener*> m_EventListeners;
    
};
#endif /* defined(__YorkshireTea__eventSystem__) */
