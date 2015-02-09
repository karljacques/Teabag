#ifndef playerComponent_h__
#define playerComponent_h__

#include "engine/component/component.h"

#include "engine/core/entitySystem.h"

// Component used to identify that a given object belongs to, or is associated with, a given player
struct PlayerComponent : public Component
{
	PlayerComponent()
	{
		GUID = 0;
	}

	Component* clone( EntID ID )
	{
		PlayerComponent* comp = entitysys::createComponent<PlayerComponent>(ID);
		comp->GUID = GUID;

		return comp;
	}

	void activate()
	{

	}

	void deactivate()
	{

	}

	EntID GUID;
};
#endif // playerComponent_h__
