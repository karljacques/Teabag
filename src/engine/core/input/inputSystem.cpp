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

InputSystem::InputSystem( EventSystem* eventSys, SDL_Window* window )
{
	m_EventSystem = eventSys;
	mWindow = window;
	mWindowActive = true;

	SDL_ShowCursor( false );

}

void InputSystem::update()
{
	while( SDL_PollEvent( &m_inputEvent ) )
	{
		switch(m_inputEvent.type )
		{
			case SDL_KEYDOWN:
			{
				// Key down, create and dispatch event
				KeyboardEvent*	e = new KeyboardEvent(EV_CORE_KEY_PRESS );
				e->mPressed = true;
				e->mReleased =false;
				e->mKeycode = m_inputEvent.key.keysym.scancode;
				m_EventSystem->dispatchEvent( e );

			}
			break;

			case SDL_KEYUP:
			{
				// Key down, create and dispatch event
				KeyboardEvent*	e = new KeyboardEvent(EV_CORE_KEY_RELEASE );
				e->mPressed = false;
				e->mReleased = true;
				e->mKeycode = m_inputEvent.key.keysym.scancode;
				m_EventSystem->dispatchEvent( e );

				//TODO Hack, so what.
				if( e->mKeycode == SDL_SCANCODE_COMMA )
					mWindowActive=!mWindowActive;

			}
			break;

			case SDL_TEXTINPUT:
				{
					KeyboardEvent*	e = new KeyboardEvent(EV_CORE_TEXT_INPUT);
					e->mKey = *m_inputEvent.text.text;
					m_EventSystem->dispatchEvent(e);
					break;
				}
				

		}
	}
	// TODO make this take half window values
	if( mWindowActive)
	{
		int MouseX,MouseY;
		SDL_GetMouseState(&MouseX,&MouseY);
		int DeltaX = MouseX - 400;
		int DeltaY = MouseY - 300;
		MouseEvent* e = new MouseEvent( EV_CORE_MOUSE_MOVEMENT );
		e->mMouseMoveX = DeltaX;
		e->mMouseMoveY = DeltaY;

		m_EventSystem->dispatchEvent( e );
		SDL_WarpMouseInWindow(  mWindow, 400, 300 );
	}
}
