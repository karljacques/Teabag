#ifndef physicsComponent_h__
#define physicsComponent_h__

#include "..\base\component.h"

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
