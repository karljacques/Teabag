#ifndef entity_h__
#define entity_h__

#include "engine/core/component/component.h"

class Entity
{
public:
	Entity(void);
	~Entity(void);

	void update( double dt );

	void addComponent( Component* component );

private:

	std::vector<Component*> mComponents;
};
#endif // entity_h__

