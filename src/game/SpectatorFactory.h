#ifndef SpectatorFactory_h__
#define SpectatorFactory_h__

#include "engine/factory/entityPrototype.h"

#include "engine/component/physics/physicsManager.h"
#include "engine/component/render/cameraManager.h"
#include "engine/component/render/renderManager.h"
#include "engine/component/player/playerManager.h"

#include "engine/component/spectator/spectatorComponent.h"
#include "engine/component/physics/physicsComponent.h"
#include "engine/component/network/networkComponent.h"
#include "engine/component/render/renderComponent.h"
#include "engine/component/render/cameraComponent.h"
#include "engine/component/player/playerComponent.h"

class SpectatorFactory
{
public:
	SpectatorFactory( );

	// Creates a spectator that is visible to the player, but not controlled by the player, controlled remotely
	Entity* createRemoteSpectator( PlayerGUID pGUID, EntGUID eGUID );

	// Creates a spectator that is controlled by the player but it is not visible, it also has a camera attached.
	Entity* createLocalSpectator( EntGUID eGUID );

protected:

	void _attachNetworkingComponents( EntID ID, PlayerGUID pGUID, EntGUID eGUID );

	EntityPrototype* mPrototype;

};
#endif // SpectatorFactory_h__