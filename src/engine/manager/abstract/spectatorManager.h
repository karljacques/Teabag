#include "..\..\core\entity-component\componentManager.h"
#include "..\..\component\abstract\spectatorComponent.h"
#include "..\..\core\event\events\keyboardEvent.h"
#include "..\..\core\event\eventSystem.h"
#include "..\..\core\entity-component\entityManager.h"

#ifndef spectatorManager_h__
#define spectatorManager_h__

class SpectatorManager : public ComponentManager<SpectatorComponent>, public EventListener
{
public:
	SpectatorManager( EntityManager* ent );

	void handle( Event* e );
	void update();
private:

	EntityManager* mEntityManager;
};
#endif // spectatorManager_h__