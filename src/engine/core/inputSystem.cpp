//
//  inputSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//
#include "pch.h"
#include "inputSystem.h"

static SDL_Window*	mWindow;
static bool			mWindowActive;
static SDL_Event	m_inputEvent;

void input::init(  SDL_Window* window )
{
	mWindow = window;
	mWindowActive = false; // start unfocused

	SDL_ShowCursor( false );

}

void input::update()
{
	while( SDL_PollEvent( &m_inputEvent ) )
	{
		switch(m_inputEvent.type )
		{
			case SDL_KEYDOWN:
			{
				if( m_inputEvent.key.repeat == false || m_inputEvent.key.keysym.scancode == SDL_SCANCODE_BACKSPACE ) // Allow backspace to have repeat strokes
				{
					// Key down, create and dispatch event
					Event*	e = eventsys::get( EVT_CORE, EV_CORE_KEY_PRESS );
					KeyboardEvent* ke = e->createEventData<KeyboardEvent>();
					ke->mPressed = true;
					ke->mReleased =false;
					ke->mKeycode = m_inputEvent.key.keysym.scancode;
					eventsys::dispatchNow( e );
				}
				
			}
			break;

			case SDL_KEYUP:
				{
					// Key down, create and dispatch event
					Event*	e = eventsys::get(EVT_CORE, EV_CORE_KEY_RELEASE );
					KeyboardEvent* ke = e->createEventData<KeyboardEvent>();
					ke->mPressed = false;
					ke->mReleased = true;
					ke->mKeycode = m_inputEvent.key.keysym.scancode;
					eventsys::dispatchNow( e );
					
				}
			break;

			case SDL_TEXTINPUT:
				{
					Event* e = eventsys::get(EVT_CORE, EV_CORE_TEXT_INPUT );
					KeyboardEvent* ke = e->createEventData<KeyboardEvent>();
					ke->mKey = *m_inputEvent.text.text;
					eventsys::dispatchNow(e);
					
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
					Event* e = eventsys::get( EVT_CORE, EV_CORE_MOUSE_PRESS );
					MouseEvent* m = e->createEventData<MouseEvent>();
					m->mMouseButton = m_inputEvent.button.button;
					eventsys::dispatchNow(e);
				}
			break;
				

		}
	}

	// Get half window height and width
	int x = 0;
	int y = 0;
	SDL_GetWindowSize (mWindow,&x,&y);

	x/=2;
	y/=2;

	
	// If Window is focused, center the mouse
	if( mWindowActive )
	{
		int MouseX,MouseY;
		SDL_GetMouseState(&MouseX,&MouseY);
		int DeltaX = MouseX - x;
		int DeltaY = MouseY - y;
		if( DeltaX != 0 || DeltaY != 0 )
		{
			Event* e = eventsys::get( EVT_CORE, EV_CORE_MOUSE_MOVEMENT );
			MouseEvent* m = e->createEventData<MouseEvent>();
			m->mMouseMoveX = DeltaX;
			m->mMouseMoveY = DeltaY;

			eventsys::dispatchNow( e );
		}
		
		SDL_WarpMouseInWindow(  mWindow, x, y );
	}

	
}
