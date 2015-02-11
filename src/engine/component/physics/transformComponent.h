#ifndef transformComponent_h__
#define transformComponent_h__

#include "common.h"

#include "engine/component/component.h"

#include "engine/core/entitySystem.h"

namespace entitysys
{
	template <class T>
	T* createComponent( EntID ID );
}

class TransformComponent : public Component
{
public:
	float3 position;
	Quat orientation;

	TransformComponent()
	{
		position = float3(0,0,0);
		orientation = Quat(0,0,0,1);
	}

	Component* clone( EntID ID )
	{
		TransformComponent* comp = entitysys::createComponent<TransformComponent>(ID);
		comp->position = position;
		comp->orientation = orientation;

		return comp;
	}

	void activate()
	{}

	void deactivate()
	{}
};

#endif // transformComponent_h__
