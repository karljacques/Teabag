#ifndef spectatorManager_h__
#define spectatorManager_h__

#include "spectatorComponent.h"
#include "engine/component/physics/physicsComponent.h"
#include "engine/component/player/playerComponent.h"

#include "engine/component/componentManager.h"

#include "engine/manager.h"

#include "engine/core/eventSystem.h"
#include "engine/core/entitySystem.h"

#include "engine/core/event/events/keyboardEvent.h"
#include "engine/core/event/events/mouseEvent.h"

class SpectatorManager : public ComponentManager, public EventListener, public Manager
{
public:
	SpectatorManager();

	void handle( Event* e );
	void update( double dt );

};
#endif // spectatorManager_h__