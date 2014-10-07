#ifndef entity_h__
#define entity_h__

#include "engine/core/component/base/component.h"

class Entity
{
public:
	Entity(void);
	~Entity(void);

	void update( double dt );

	void addComponent( Component* component );
	void listenToAll( EventListener* listener );
	void makeListenToMe( Component* listenTo );

private:

	std::vector<Component*> mComponents;
};
#endif // entity_h__

