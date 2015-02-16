#include "pch.h"
#include "playerManager.h"

PlayerManager::PlayerManager()
{
	// Get my GUID
	mLocalPlayer= network::getPlayerGUID();

	// Add local player to the list
	std::shared_ptr<Player> local = std::shared_ptr<Player>( new Player() );
	local->pGUID = mLocalPlayer;
	this->addPlayer( local );

	::printm("Local GUID:" + std::to_string(local->pGUID));
}

void PlayerManager::addPlayer(PlayerGUID guid, std::string username)
{
	// Construct player
	std::shared_ptr<Player> player( new Player() );
	player->pGUID = guid;
	player->username = username;

	// Copy the shared ptr to the map
	mConnectedPlayers[guid] = player;
}

void PlayerManager::addPlayer(std::shared_ptr<Player> player)
{
	// If object constructed elsewhere
	mConnectedPlayers[player->pGUID] = player;
}

bool PlayerManager::playerExists(PlayerGUID guid)
{
	return ( mConnectedPlayers.count(guid) > 0 );
}

std::shared_ptr<Player> PlayerManager::getPlayerByGUID(PlayerGUID guid)
{
	if( mConnectedPlayers.count(guid) > 0 )
		return mConnectedPlayers[guid];
	else
	{
		assert( "Should be checking that player exists!!");
		return nullptr;
	}
}

void PlayerManager::removePlayerByGUID(PlayerGUID guid)
{
	mConnectedPlayers.erase( guid );
}

void PlayerManager::constructFromData(char* data)
{
	// TODO
}

void PlayerManager::handle(Event* e)
{
	switch( e->getEventType() )
	{
	case EV_NETWORK_PLAYER_DATA:
		{
			// Add new player - this will only be called when I am host
			PlayerEvent* p = e->getData<PlayerEvent>();
			this->addPlayer( p->pGUID, std::string( p->username ) );

			::printm("New Player Connecting: " + std::string( p->username ) + "  with GUID: " + std::to_string( p->pGUID ) );

			// As I am host, inform clients of new player - dispatch local event too.
			// Clone event, send to all under 'EV_NETWORK_PLAYER_JOINED'
			Event* newEvent = eventsys::get( EV_NETWORK_PLAYER_JOINED );

			newEvent->clone( e );
			network::sendEvent( newEvent, HIGH_PRIORITY, RELIABLE );
			eventsys::dispatch(newEvent);

			break;
		}
	case EV_NETWORK_PLAYER_JOINED:
		{
			PlayerEvent* p = e->getData<PlayerEvent>();

			// Check if this is a notification that the host has acknowledged the current player
			if( p->pGUID == getLocalPlayer()->pGUID )
			{
				::printm("Host has acknowledged your player connection.");
			}
			else
			{
				this->addPlayer(p->pGUID, std::string( p->username ) );
				::printm( "New player has joined: " + std::string( p->username ) );

				// Send message to listeners that the client is ready for init data
				// This is where you can send things like spawners and level load stuff, for example.
				Event* initEvent = eventsys::get( EV_NETWORK_INIT_READY );
				PlayerEvent* playerEvent = initEvent->createEventData<PlayerEvent>();
				playerEvent->pGUID = p->pGUID;
				eventsys::dispatch(initEvent);
			}
			break;
		}

	case EV_NETWORK_NEW_CONNECTION:
		{
			// Signal from the network system that we've connected to a host, we'll need to send it our player data
			Player* p = this->getPlayerByGUID(mLocalPlayer).get();
			Event* ne = new Event( EV_NETWORK_PLAYER_DATA );
			PlayerEvent* pe = ne->createEventData<PlayerEvent>();

			assert( p->username.size() < USERNAME_LENGTH );

			memcpy( pe->username, p->username.c_str(), p->username.size() + 1 );
			pe->pGUID = mLocalPlayer;

			network::sendEvent(ne, HIGH_PRIORITY,RELIABLE );
			delete ne;
			break;
		}
	}
}

void PlayerManager::update(double dt)
{

}

std::shared_ptr<Player> PlayerManager::getLocalPlayer()
{
	return mConnectedPlayers[mLocalPlayer];
}

PlayerGUID PlayerManager::getLocalGUID()
{
	return mLocalPlayer;
}


