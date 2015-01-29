#include "../../core/entity-component/componentManager.h"
#include "../../component/abstract/spectatorComponent.h"
#include "../../core/event/events/keyboardEvent.h"
#include "../../core/event/eventSystem.h"
#include "../../core/entity-component/entityManager.h"
#include "../manager.h"

#ifndef spectatorManager_h__
#define spectatorManager_h__

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