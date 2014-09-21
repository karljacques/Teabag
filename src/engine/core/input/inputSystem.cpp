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
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				mWindowActive = true;
				break;

			case SDL_WINDOWEVENT_HIDDEN:
				mWindowActive = false;
				break;

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

				}
			break;

			case SDL_TEXTINPUT:
				{
					KeyboardEvent*	e = new KeyboardEvent(EV_CORE_TEXT_INPUT);
					e->mKey = *m_inputEvent.text.text;
					m_EventSystem->dispatchEvent(e);
					break;
				}

			case SDL_WINDOWEVENT:
				switch( m_inputEvent.window.event )
				{
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					mWindowActive = true;
					break;

				case SDL_WINDOWEVENT_FOCUS_LOST:
					mWindowActive = false;
					break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				{
					MouseEvent* m = new MouseEvent(EV_CORE_MOUSE_PRESS);
					m->m_MouseButton = m_inputEvent.button.button;
					m_EventSystem->dispatchEvent(m);
				}
				
				

		}
	}

	// Get half window height and width
	int x;
	int y;
	SDL_GetWindowSize (mWindow,&x,&y);

	x/=2;
	y/=2;

	// If Window is focused, center the mouse
	if( mWindowActive)
	{
		int MouseX,MouseY;
		SDL_GetMouseState(&MouseX,&MouseY);
		int DeltaX = MouseX - x;
		int DeltaY = MouseY - y;
		MouseEvent* e = new MouseEvent( EV_CORE_MOUSE_MOVEMENT );
		e->mMouseMoveX = DeltaX;
		e->mMouseMoveY = DeltaY;

		m_EventSystem->dispatchEvent( e );
		SDL_WarpMouseInWindow(  mWindow, x, y );
	}
}
