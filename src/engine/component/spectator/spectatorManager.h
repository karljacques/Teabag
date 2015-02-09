#ifndef spectatorManager_h__
#define spectatorManager_h__

#include "spectatorComponent.h"
#include "engine/component/physics/physicsComponent.h"

#include "engine/component/componentManager.h"
#include "engine/component/entity/entityManager.h"
#include "engine/manager.h"

#include "engine/core/eventSystem.h"

#include "engine/core/event/events/keyboardEvent.h"
#include "engine/core/event/events/mouseEvent.h"

class SpectatorManager : public ComponentManager, public EventListener, public Manager
{
public:
	SpectatorManager( EntityManager* ent );

	void handle( Event* e );
	void update( double dt );
private:

	EntityManager* mEntityManager;
};
#endif // spectatorManager_h__