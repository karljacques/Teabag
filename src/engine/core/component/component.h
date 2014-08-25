#ifndef component_h__
#define component_h__

#include "engine/core/event/EventListener.h"

class Entity;

class Component
{
public:
	Component(void);
	~Component(void);

	virtual void update( double dt ) = 0;

	void registerListener( EventListener* comp );
	void deregisterListener( EventListener* comp );

	void dispatch( Event* e );
	
protected:
	// Components listening for updates on this one
	std::vector<EventListener*> mListeners;

	// Entity that this component belongs to - perhaps my comments state the obvious too much.
	Entity* mParent;
};

#endif // component_h__
