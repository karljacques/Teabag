#include "pch.h"
#include "playerManager.h"

PlayerManager::PlayerManager()
{
	// Get my GUID
	mLocalPlayer= network::getPlayerGUID();

	// Add local player to the list
	std::shared_ptr<Player> local = std::shared_ptr<Player>( new Player() );
	local->GUID = mLocalPlayer;
	this->addPlayer( local );

	::printm("Local GUID:" + std::to_string(local->GUID));
}

void PlayerManager::addPlayer(EntID guid, std::string username)
{
	// Construct player
	std::shared_ptr<Player> player( new Player() );
	player->GUID = guid;
	player->username = username;

	// Copy the shared ptr to the map
	mConnectedPlayers[guid] = player;
}

void PlayerManager::addPlayer(std::shared_ptr<Player> player)
{
	// If object constructed elsewhere
	mConnectedPlayers[player->GUID] = player;
}

bool PlayerManager::playerExists(EntID guid)
{
	return ( mConnectedPlayers.count(guid) > 0 );
}

std::shared_ptr<Player> PlayerManager::getPlayerByGUID(EntID guid)
{
	if( mConnectedPlayers.count(guid) > 0 )
		return mConnectedPlayers[guid];
	else
	{
		assert( "Should be checking that player exists!!");
		return nullptr;
	}
}

void PlayerManager::removePlayerByGUID(EntID guid)
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

			// As I am host, inform clients of new player
			// Clone event, send to all under 'EV_NETWORK_PLAYER_JOINED'
			Event* ne = new Event( EV_NETWORK_PLAYER_JOINED );
#ifdef _DEBUG
			ne->d_initialised = true;
#endif
			ne->clone( e );
			network::sendEvent( ne, HIGH_PRIORITY, RELIABLE );
			delete ne;

			break;
		}
	case EV_NETWORK_PLAYER_JOINED:
		{
			PlayerEvent* p = e->getData<PlayerEvent>();

			// Check if this is a notification that the host has acknowledged the current player
			if( p->pGUID == getLocalPlayer()->GUID )
			{
				::printm("Host has acknowledged your player connection.");
			}
			else
			{
				this->addPlayer(p->pGUID, std::string( p->username ) );
				::printm( "New player has joined: " + std::string( p->username ) );
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
	case EV_CORE_TRANSFORM_UPDATE_ORIENTATION:
		// This means the client has rotated their camera, thus rotating their entire orientation for movement and visuals
		// Need to send this orientation change to the host in the form of a snapshot update.
		// Check that the object is an object owned by the player.
		// Get the player
		std::shared_ptr<Player> local = this->getLocalPlayer();

		// See if the object the event refers to is owned by this player.
		if( local->isOwner( e->ID ) )
		{
			// Get the GUID of the object - fail if no network component
			//if( this->mNetworkSystem->attach_eGUID(e) )
			//{
				// Client owns this object, it is a networked object. Update Transform system to reflect changes.
			//	this->mNetworkSystem->getShapshotManager()->handle(e);
			}
		//}

	}
}

void PlayerManager::update(double dt)
{

}

std::shared_ptr<Player> PlayerManager::getLocalPlayer()
{
	return mConnectedPlayers[mLocalPlayer];
}



bool Player::isOwner(EntID ID)
{
	return( mComponents.count(ID) > 0 );
}
