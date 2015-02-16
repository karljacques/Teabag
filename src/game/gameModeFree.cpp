#include "pch.h"
#include "gameModeFree.h"

GameModeFree::GameModeFree()
{
	// Create the player spectator
	Entity* spectator = mSpectatorFactory.createLocalSpectator( network::getNetworkManager()->assignGUID() );
	mEntities.push_back(spectator);
}

GameModeFree::~GameModeFree()
{

}

FrameState GameModeFree::update()
{
	return mFrameState;
}

FrameState GameModeFree::backgroundUpdate()
{
	return mFrameState;
}


void GameModeFree::handle(Event* e)
{
	switch( e->getEventType() )
	{
	case EV_CORE_KEY_PRESS:
		{
			KeyboardEvent* ke = e->getData<KeyboardEvent>();
			switch (ke->mKeycode)
			{
			case SDL_SCANCODE_ESCAPE: // Escape
				mFrameState = FR_BEGIN_TERMINATION;
				break;
			}
		}
		break;

	// New player has joined, create their spectator
	case EV_SPAWN_SPECTATOR:
		{
			// Get the event data
			PlayerEvent* pEvent = e->getData<PlayerEvent>();
			Entity* spectator = mSpectatorFactory.createRemoteSpectator(pEvent->pGUID, 0 );
			mEntities.push_back( spectator );
		}

	// New client is ready for spawn data and the likes, send it.
	case EV_NETWORK_INIT_READY:
	}
}

void GameModeFree::reset()
{
	deinit();
}

void GameModeFree::deinit()
{

}





