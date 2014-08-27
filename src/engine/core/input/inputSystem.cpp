//
//  inputSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//

#include <SDL.h>
#include "inputSystem.h"
#include "../event/eventSystem.h"

InputSystem::InputSystem( EventSystem* eventSys )
{
	m_EventSystem = eventSys;

}

void InputSystem::update()
{
	if( SDL_PollEvent( &m_inputEvent ) )
	{
		if( m_inputEvent.type == SDL_KEYDOWN )
		{
			// Key down, create and dispatch event
			KeyboardEvent*	e = new KeyboardEvent(EV_KeyPress );
			e->mPressed = true;
			e->mReleased =false;
			e->mKeycode = m_inputEvent.key.keysym.scancode;
			m_EventSystem->dispatchEvent( e );
		}

		if( m_inputEvent.type == SDL_KEYUP )
		{
			// Key down, create and dispatch event
			KeyboardEvent*	e = new KeyboardEvent(EV_KeyPress );
			e->mPressed = false;
			e->mReleased = true;
			e->mKeycode = m_inputEvent.key.keysym.scancode;
			m_EventSystem->dispatchEvent( e );
		}
	}
}
