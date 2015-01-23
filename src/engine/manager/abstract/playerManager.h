#include <list>

#include "../manager.h"
#include "../../core/entity-component/componentManager.h"
#include "../../component/abstract/playerComponent.h"
#include "../../core/entity-component/entityManager.h"
#include "../network/networkSystem.h"

#ifndef playerManager_h__
#define playerManager_h__

using namespace std;

class Player : public ComponentManager<PlayerComponent>
{
public:
	Player()
	{
		GUID = 0;
		username = "nebuchadnezzar";
	}

	EntID GUID;
	std::string username;
};


class PlayerManager: public Manager, public EventListener
{
public:
	PlayerManager( NetworkSystem* networkSystem );

	// Add new players that join after initial connection
	void	addPlayer( EntID guid, std::string username );
	void	addPlayer( shared_ptr<Player> player );

	// Get and remove players
	shared_ptr<Player> getPlayerByGUID( EntID guid );
	bool	playerExists( EntID guid );
	void	removePlayerByGUID( EntID guid );

	// EventListener handle
	void	handle( Event* e );

	// Update
	void	update( double dt );

	// Used when this client initially joins the game, takes initial player data
	// and turns it into usable form.
	void	constructFromData( char* data );

private:

	std::map<EntID, shared_ptr<Player>> mConnectedPlayers;
	EntID mLocalPlayer;

	NetworkSystem* mNetworkSystem;


};
#endif // playerManager_h__
