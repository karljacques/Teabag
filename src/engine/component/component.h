#ifndef component_h__
#define component_h__

#include "common.h"

class Component 
{
public:
	Component(void);
	virtual ~Component(void);

	virtual Component* clone( EntID ID ) = 0;
	virtual void activate() = 0;
	virtual void deactivate() = 0;

	/* Local identifier, used to sync components */
	EntID ID;

};

#endif // component_h__
