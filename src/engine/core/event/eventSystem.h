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
#include "eventListener.h"


class EventSystem
{
public:

    EventSystem();

    void dispatchEvent( Event* e );

    void handleEvents();

    void registerListener( EventListener* e );
    void deregisterListener( EventListener* e );

private:

    std::queue<Event*> mEventList;
    std::deque<EventListener*> mEventListeners;

};
#endif /* defined(__YorkshireTea__eventSystem__) */
