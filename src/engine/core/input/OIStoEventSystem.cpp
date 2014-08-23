//
//  OIStoEventSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//

#include "OIStoEventSystem.h"
#include "../event/eventSystem.h"

OISToEventSystem::OISToEventSystem()
{
    mEventSystem = 0;
}

bool OISToEventSystem::keyPressed(const OIS::KeyEvent &e )
{
    KeyboardEvent* ev = new KeyboardEvent( EV_KeyPress );
    ev->mPressed = true;
    ev->mKeycode = e.key;
    
    mEventSystem->dispatchEvent(ev);
    return true;
}

bool OISToEventSystem::keyReleased( const OIS::KeyEvent &e )
{
    KeyboardEvent* ev = new KeyboardEvent( EV_KeyPress );
    ev->mReleased = true;
    ev->mKeycode = e.key;
    
    mEventSystem->dispatchEvent(ev);
    return true;
}

bool OISToEventSystem::mouseMoved(const OIS::MouseEvent &e )
{
    
}

bool OISToEventSystem::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ){
    
}

bool OISToEventSystem::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ){
    
}

void OISToEventSystem::injectEventSystem( EventSystem* m )
{
    mEventSystem = m;
}