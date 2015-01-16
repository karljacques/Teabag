#include "../manager.h"
#include "../../core/entity-component/componentManager.h"
#include "../../component/abstract/playerComponent.h"
#include <list>
#include "../../core/entity-component/entityManager.h"
#include "../network/networkSystem.h"

#ifndef playerManager_h__
#define playerManager_h__

class Player
{
	EntID GUID;
	std::string username;
};


class PlayerManager: public Manager, public ComponentManager<PlayerComponent>, EventListener
{
public:
	PlayerManager( NetworkSystem* mNetworkSystem );

private:

	std::map<EntID, Player*> mConnectedPlayers;
	EntID mLocalGUID;


};
#endif // playerManager_h__
