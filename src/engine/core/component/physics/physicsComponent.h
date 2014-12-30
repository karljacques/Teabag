#ifndef physicsComponent_h__
#define physicsComponent_h__

#include "engine/core/component/base/component.h"
#include "engine/core/physics/physicsManager.h"

#include "engine/core/event/eventListener.h"
#include "common.h"

class PhysicsComponent :public Component
{
public:
	PhysicsComponent( ){};
	~PhysicsComponent(){};

	btRigidBody* body;

	float3 position;
	Quat orientation;

};

#endif // physicsComponent_h__
