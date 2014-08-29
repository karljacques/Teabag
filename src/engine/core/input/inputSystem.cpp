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

	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void InputSystem::update()
{
	if( SDL_PollEvent( &m_inputEvent ) )
	{
		switch(m_inputEvent.type )
		{
			case SDL_KEYDOWN:
			{
				// Key down, create and dispatch event
				KeyboardEvent*	e = new KeyboardEvent(EV_KEY_PRESS );
				e->mPressed = true;
				e->mReleased =false;
				e->mKeycode = m_inputEvent.key.keysym.scancode;
				m_EventSystem->dispatchEvent( e );
			}
			break;

			case SDL_KEYUP:
			{
				// Key down, create and dispatch event
				KeyboardEvent*	e = new KeyboardEvent(EV_KEY_PRESS );
				e->mPressed = false;
				e->mReleased = true;
				e->mKeycode = m_inputEvent.key.keysym.scancode;
				m_EventSystem->dispatchEvent( e );
			}
			break;

			case SDL_MOUSEMOTION:
				{
					MouseEvent* e = new MouseEvent( EV_MOUSE_MOVEMENT );
					e->mMouseMoveX = m_inputEvent.motion.xrel;
					e->mMouseMoveY = m_inputEvent.motion.yrel;
					e->m_MouseX = (int)m_inputEvent.motion.x;
					e->m_MouseY = (int)m_inputEvent.motion.y;
					m_EventSystem->dispatchEvent( e );
				}

		}
	}
}
