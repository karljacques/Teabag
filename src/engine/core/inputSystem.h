//
//  inputSystem.h
//  Teabag
//
//  Created by Karl Jacques on 24/05/2014.
//
//

#ifndef __YorkshireTea__inputSystem__
#define __YorkshireTea__inputSystem__

#include <iostream>
#include "engine/core/eventSystem.h"
#include "engine/manager.h"

#include "engine/core/event/events/keyboardEvent.h"
#include "engine/core/event/events/mouseEvent.h"
namespace input
{
	void init(  SDL_Window* window );
	void update();
}

#endif /* defined(__YorkshireTea__inputSystem__) */
