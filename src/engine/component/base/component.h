#ifndef component_h__
#define component_h__

#include "common.h"
#include "engine/core/event/eventListener.h"

class Entity;

class Component 
{
public:
	Component(void);
	virtual ~Component(void);

	/* Global identifier, the same on every networked system */
	unsigned int GUID;

	/* Local identifier, used to sync components */
	unsigned int LUID;

};

#endif // component_h__
