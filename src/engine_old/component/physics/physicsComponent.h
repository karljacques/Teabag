#ifndef physicsComponent_h__
#define physicsComponent_h__

#include "../base/component.h"

class PhysicsComponent :public Component
{
public:
	PhysicsComponent( ){
		position = float3(0,0,0);
		orientation = Quat(0,0,0,1);
	};
	~PhysicsComponent(){};

	btRigidBody* body;

	float3 position;
	Quat orientation;

	Component* clone()
	{
		PhysicsComponent* comp = new PhysicsComponent();
		comp->position = position;
		comp->orientation = orientation;

		// Clone the body, which belongs to bullet.
		return new PhysicsComponent();
	}
};

#endif // physicsComponent_h__
