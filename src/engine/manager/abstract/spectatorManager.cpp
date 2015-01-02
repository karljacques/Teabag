#include "pch.h"
#include "spectatorManager.h"

void SpectatorManager::handle( Event* e )
{
	/* Transform key presses into forces */
	if( e->getEventType() == EV_CORE_KEY_PRESS )
	{
		float3 dir;
		float speed = 1.0f;

		// Cast to key press
		KeyboardEvent* k = e->getData<KeyboardEvent>();

		switch( k->mKeycode )
		{
		case SDL_SCANCODE_W:
			dir+=float3(0,0,1);
			break;
		case SDL_SCANCODE_A:
			dir+=float3(-1,0,0);
			break;
		case SDL_SCANCODE_S:
			dir+=float3(0,0,-1);
			break;
		case SDL_SCANCODE_D:
			dir+=float3(1,0,0);
			break;
		}

		/* Loop through spectators and apply */
		for( auto i=mComponents.begin(); i!=mComponents.end();i++ )
		{
			i->second->dir = dir;
			i->second->speed = speed;

			// Apply force
			Event* ne = EventSystem::getSingletonPtr()->getEvent(EV_CORE_APPLY_FORCE);
			ne->LUID = i->second->LUID;
			TransformEvent* te = ne->createEventData<TransformEvent>();
			te->mFloat3_1 = dir*speed;
			EventSystem::getSingletonPtr()->dispatchEvent(ne);
		}
	}
}