#include "pch.h"
#include "gameModeFree.h"

GameModeFree::GameModeFree()
{
	// Create the player spectator
	Entity* spectator = mSpectatorFactory.createLocalSpectator( network::getNetworkManager()->assignGUID() );
	mSpectators.push_back(spectator);
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
			printm( "Spawned another spectator" );

			// Get the event data
			SpawnEvent* sEvent = e->getData<SpawnEvent>();

			printm( "pGUID: " + std::to_string( sEvent->pGUID ) );
			if( sEvent->pGUID == network::getPlayerGUID() )
			{
				reset();
				printm( "Created Local Spectator with eGUID " + std::to_string( sEvent->eGUID ));
				Entity* spectator = mSpectatorFactory.createLocalSpectator( sEvent->eGUID );
				mSpectators.push_back( spectator );
			}
			else
			{
				printm( "Created Remote Spectator with eGUID " + std::to_string( sEvent->eGUID ));
				Entity* spectator = mSpectatorFactory.createRemoteSpectator(sEvent->pGUID, sEvent->eGUID );
				mSpectators.push_back( spectator );
			}
		}
		break;

	// New client is ready for spawn data and the likes, send it.
	case EV_NETWORK_INIT_READY:
		{
			printm( "Init client");
			PlayerEvent* p = e->getData<PlayerEvent>();

			// Tell the client about the already existing spectators
			for( auto i=mSpectators.begin(); i!=mSpectators.end(); i++ )
			{
				// Tell the client to create it's spectator
				Event* specCreate = eventsys::get( EV_SPAWN_SPECTATOR );
				SpawnEvent* spawnEvent = specCreate->createEventData<SpawnEvent>();

				spawnEvent->pGUID = (*i)->getComponent<PlayerComponent>()->pGUID;
				spawnEvent->eGUID = (*i)->getComponent<NetworkComponent>()->eGUID;

				network::sendEvent( specCreate, HIGH_PRIORITY, RELIABLE, RakNet::RakNetGUID( p->pGUID ) );
			}

			// Tell the client to create it's spectator
			Event* specCreate = eventsys::get( EV_SPAWN_SPECTATOR );
			SpawnEvent* spawnEvent = specCreate->createEventData<SpawnEvent>();

			spawnEvent->pGUID = p->pGUID;
			spawnEvent->eGUID = network::getNetworkManager().get()->assignGUID();

			// Send event network
			network::sendEvent( specCreate, IMMEDIATE_PRIORITY, RELIABLE );

			// Dispatch
			eventsys::dispatch( specCreate );

			

		}
		break;
	}
}

void GameModeFree::reset()
{
	deinit();
}

void GameModeFree::deinit()
{
	while( mSpectators.size() > 0 )
	{
		Entity* back = mSpectators.back();
		mSpectators.pop_back();

		entitysys::destroyEntity(back);
	}
}





