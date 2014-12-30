#ifndef component_h__
#define component_h__

#include "common.h"
#include "engine/core/event/eventListener.h"

class Entity;

class Component 
{
public:
	Component(void);
	~Component(void);

	/* Global identifier, the same on every networked system */
	int GUID;

};

#endif // component_h__
