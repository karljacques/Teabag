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
#include <Math/float3.h>
#include <Math/Quat.h>

enum EV_EventType
{
    EV_MOUSEMOVE,
    EV_MOUSE_PRESS,
    EV_MOUSE_RELEASE,
    EV_KEY_PRESS,
    EV_KEY_RELEASE,
	EV_MOVEMENT,
	EV_ROTATION
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

class MovementEvent : public Event
{
public:
	MovementEvent( int EV_EventType );

	float3 mPosition;
	Quat mOrientation;
};

#endif /* defined(__YorkshireTea__event__) */
