#ifndef entity_h__
#define entity_h__

#include "engine/core/component/base/component.h"

class Entity
{
public:
	Entity(void);
	~Entity(void);

	void addComponent( Component* c );
	void removeComponent( Component* c );

	template <class T>
	T* getComponent();

	template <class T>
	bool hasComponent();

	unsigned int LUID;
	unsigned int GUID;

private:
	std::vector<Component*> mComponents;
};



template <class T>
T* Entity::getComponent()
{
	for( auto i = mComponents.begin(); i != mComponents.end(); i++ )
	{
		if( typeid(**i) == typeid(T) )
		{
			return static_cast<T*>(*i);
		}
	}

	return nullptr;
}
template <class T>
bool Entity::hasComponent()
{
	for( auto i = mComponents.begin(); i != mComponents.end(); i++ )
	{
		if typeid( (**i) == typeid(T) )
		{
			return true;
		}
	}
	return false;
}

#endif // entity_h__