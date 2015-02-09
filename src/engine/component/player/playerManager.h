#ifndef playerManager_h__
#define playerManager_h__

#include "playerComponent.h"

#include "engine/manager.h"
#include "engine/component/componentManager.h"
#include "engine/component/entity/entityManager.h"
#include "engine/manager/debug/ogreConsole.h"

#include "engine/core/eventSystem.h"
#include "engine/core/networkSystem.h"

#include "engine/core/event/events/playerEvent.h"

class Player : public ComponentManager
{
public:
	Player()
	{
		GUID = 0;
		username = "nebuchadnezzar";
	}

	bool isOwner( EntID ID );

	EntID GUID;
	std::string username;
};


class PlayerManager: public Manager, public EventListener
{
public:
	PlayerManager( );

	// Add new players that join after initial connection
	void	addPlayer( EntID guid, std::string username );
	void	addPlayer( std::shared_ptr<Player> player );

	// Get and remove players
	std::shared_ptr<Player> getPlayerByGUID( EntID guid );
	bool	playerExists( EntID guid );
	void	removePlayerByGUID( EntID guid );

	// PlayerManager statistics
	unsigned int getNumberOfConnectedPlayers();

	// Current player manipulation
	std::shared_ptr<Player>	getLocalPlayer();
	EntID				getLocalGUID();

	// EventListener handle
	void	handle( Event* e );

	// Update
	void	update( double dt );

	// Used when this client initially joins the game, takes initial player data
	// and turns it into usable form.
	void	constructFromData( char* data );

private:

	std::map<EntID, std::shared_ptr<Player>> mConnectedPlayers;
	EntID mLocalPlayer;


};
#endif // playerManager_h__
