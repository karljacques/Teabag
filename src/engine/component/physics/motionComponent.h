#ifndef motionComponent_h__
#define motionComponent_h__

#include "common.h"

#include "engine/component/component.h"

#include "engine/core/entitySystem.h"

namespace entitysys
{
	template <class T>
	T* createComponent( EntID ID );
}

struct MotionComponent : public Component
{
	float3 velocity;
	float3 angularVelocity;

	Component* clone(  EntID ID  )
	{
		MotionComponent* comp = entitysys::createComponent<MotionComponent>(ID);
		comp->velocity = velocity;
		comp->angularVelocity = angularVelocity;

		return comp;
	}

	void activate()
	{

	}

	void deactivate()
	{

	}

};

#endif // motionComponent_h__


