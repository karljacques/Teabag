#include "..\..\core\entity-component\componentManager.h"
#include "..\..\component\abstract\spectatorComponent.h"
#include "..\..\core\event\events\keyboardEvent.h"
#include "..\..\core\event\eventSystem.h"

#ifndef spectatorManager_h__
#define spectatorManager_h__

class SpectatorManager : public ComponentManager<SpectatorComponent>, public EventListener
{
	void handle( Event* e );
}
#endif // spectatorManager_h__