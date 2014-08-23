//
//  event.h
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//

#ifndef __YorkshireTea__event__
#define __YorkshireTea__event__

#include <iostream>

enum EV_EventType
{
    EV_MouseMove,
    EV_MousePress,
    EV_MouseRelease,
    EV_KeyPress,
    EV_KeyRelease
};

class Event
{

public:

    Event( int eventType );

    int     getEventType();

private:

    int     mEventType;

};

class MouseEvent : public Event
{
public:
    MouseEvent( int eventType );

    int m_MouseMoveX;
    int m_MouseMoveY;
    int m_MouseMoveZ;

    int m_MouseX;
    int m_MouseY;
    int m_MouseZ;

    int m_MouseButton;
    bool m_Released;
    bool m_Pressed;
};

class KeyboardEvent : public Event
{
public:
    KeyboardEvent( int eventType );

    bool mPressed;
    bool mReleased;

    int mKeycode;
};

#endif /* defined(__YorkshireTea__event__) */
