#ifndef playerComponent_h__
#define playerComponent_h__

#include "engine/component/component.h"

// Component used to identify that a given object belongs to, or is associated with, a given player
struct PlayerComponent : public Component
{
	PlayerComponent()
	{
		GUID = 0;
	}

	Component* clone()
	{
		PlayerComponent* comp = new PlayerComponent();
		comp->GUID = GUID;

		return comp;
	}
	EntID GUID;
};
#endif // playerComponent_h__
