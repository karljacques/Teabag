#ifndef component_h__
#define component_h__

class Entity;

class Component
{
public:
	Component(void);
	~Component(void);

	virtual void update( double dt ) = 0;

	Entity* mParent;
};

#endif // component_h__
