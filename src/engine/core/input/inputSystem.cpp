//
//  inputSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//
#include "pch.h"
#include <SDL.h>
#include "inputSystem.h"
#include "../event/eventSystem.h"
#include "engine/core/graphics/ui/ogreConsole.h"

InputSystem::InputSystem(  SDL_Window* window )
{
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
				Event*	e = EventSystem::getSingletonPtr()->getEvent(EV_CORE_KEY_PRESS );
				KeyboardEvent* ke = e->createEventData<KeyboardEvent>();
				ke->mPressed = true;
				ke->mReleased =false;
				ke->mKeycode = m_inputEvent.key.keysym.scancode;
				EventSystem::getSingletonPtr()->dispatchEvent( e );
				
			}
			break;

			case SDL_KEYUP:
				{
					// Key down, create and dispatch event
					Event*	e = EventSystem::getSingletonPtr()->getEvent(EV_CORE_KEY_RELEASE );
					KeyboardEvent* ke = e->createEventData<KeyboardEvent>();
					ke->mPressed = false;
					ke->mReleased = true;
					ke->mKeycode = m_inputEvent.key.keysym.scancode;
					EventSystem::getSingletonPtr()->dispatchEvent( e );
					
				}
			break;

			case SDL_TEXTINPUT:
				{
					Event* e = EventSystem::getSingletonPtr()->getEvent(EV_CORE_TEXT_INPUT );
					KeyboardEvent* ke = e->createEventData<KeyboardEvent>();
					ke->mKey = *m_inputEvent.text.text;
					EventSystem::getSingletonPtr()->dispatchEvent(e);
					
				}
			break;
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
					Event* e = EventSystem::getSingletonPtr()->getEvent( EV_CORE_MOUSE_PRESS );
					MouseEvent* m = e->createEventData<MouseEvent>();
					m->mMouseButton = m_inputEvent.button.button;
					EventSystem::getSingletonPtr()->dispatchEvent(e);
				}
			break;
				

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
		if( DeltaX != 0 || DeltaY != 0 )
		{
			Event* e = EventSystem::getSingletonPtr()->getEvent( EV_CORE_MOUSE_MOVEMENT );
			MouseEvent* m = e->createEventData<MouseEvent>();
			m->mMouseMoveX = DeltaX;
			m->mMouseMoveY = DeltaY;

			EventSystem::getSingletonPtr()->dispatchEvent( e );
		}
		
		SDL_WarpMouseInWindow(  mWindow, x, y );
	}
	
}
