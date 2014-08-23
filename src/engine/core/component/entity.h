#ifndef entity_h__
#define entity_h__

#include "engine/core/component/renderComponent.h"
#include "engine/core/component/physicsComponent.h"

class Entity
{
public:
	Entity(void);
	~Entity(void);

	RenderComponent* renderComponent;
	PhysicsComponent* physicsComponent;
	
	Ogre::Vector3 mPosition;

	void update( double dt );
};
#endif // entity_h__

