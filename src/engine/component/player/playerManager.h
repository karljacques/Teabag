#ifndef playerManager_h__
#define playerManager_h__

#include "playerComponent.h"

#include "engine/manager.h"
#include "engine/component/componentManager.h"

#include "engine/manager/debug/ogreConsole.h"

#include "engine/core/eventSystem.h"
#include "engine/core/networkSystem.h"
#include "engine/core/entitySystem.h"

#include "engine/core/event/events/playerEvent.h"

class Player 
{
public:
	Player()
	{
		pGUID = 0;
		username = "nebuchadnezzar";
	}

	PlayerGUID pGUID;
	std::string username;

};


class PlayerManager: public Manager, public EventListener, public ComponentManager
{
public:
	PlayerManager( );

	// Add new players that join after initial connection
	void	addPlayer( PlayerGUID guid, std::string username );
	void	addPlayer( std::shared_ptr<Player> player );

	// Get and remove players
	std::shared_ptr<Player> getPlayerByGUID( PlayerGUID guid );
	bool	playerExists( PlayerGUID guid );
	void	removePlayerByGUID( PlayerGUID guid );

	// PlayerManager statistics
	unsigned int getNumberOfConnectedPlayers();

	// Current player manipulation
	std::shared_ptr<Player>	getLocalPlayer();
	PlayerGUID				getLocalGUID();

	// EventListener handle
	void	handle( Event* e );

	// Update
	void	update( double dt );

	// Used when this client initially joins the game, takes initial player data
	// and turns it into usable form.
	void	constructFromData( char* data );


private:

	std::map<PlayerGUID, std::shared_ptr<Player>> mConnectedPlayers;
	PlayerGUID mLocalPlayer;


};
#endif // playerManager_h__
