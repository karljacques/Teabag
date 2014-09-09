//
//  inputSystem.h
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//

#ifndef __YorkshireTea__inputSystem__
#define __YorkshireTea__inputSystem__

#include <iostream>
#include <SDL.h>
#include "../event/eventSystem.h"

class InputSystem
{
public:
        
    InputSystem( EventSystem* eventSys, SDL_Window* window );
        
    void update();
private:
        
	SDL_Event		m_inputEvent;
    EventSystem*	m_EventSystem;

	// Window 
	SDL_Window* mWindow;
	bool mWindowActive;
};


#endif /* defined(__YorkshireTea__inputSystem__) */
