//
//  OIStoEventSystem.h
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//

#ifndef __YorkshireTea__OIStoEventSystem__
#define __YorkshireTea__OIStoEventSystem__

#include <iostream>
#include <OIS.h>
#include "../event/eventSystem.h"

class OISToEventSystem : public OIS::MouseListener, public OIS::KeyListener
{
public:
    OISToEventSystem( );
    
    void injectEventSystem( EventSystem* m );
    
    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );
    
    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    
private:
    EventSystem* mEventSystem;
};
#endif /* defined(__YorkshireTea__OIStoEventSystem__) */
