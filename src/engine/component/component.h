#ifndef component_h__
#define component_h__

#include "common.h"

class Component 
{
public:
	Component(void);
	virtual ~Component(void);

	virtual Component* clone() = 0;

	/* Local identifier, used to sync components */
	EntID ID;

};

#endif // component_h__
