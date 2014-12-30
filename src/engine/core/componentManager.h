#include "../world/entityManager.h"

template <class T>
class ComponentManager
{
public:
	ComponentManager( EntityManager* ent )
	{
		mEntityMgr = ent;
	}
	
	T* createComponent( );
	void removeComponent( T* comp );

protected:
	std::vector<T*> mComponents;
	EntityManager* mEntityMgr;

};

template <class T>
T* ComponentManager::createComponent()
{
	T* comp = new T;
	mComponents.push_back(comp);
	return comp;
}

template <class T>
void ComponentManager::removeComponent( T* comp )
{
	for( auto i = mComponents.begin(); i != mComponents.end(); i++ )
	{
		if( (*i) == comp )
		{
			mCompoents.erase(i);
			return;
		}
	}
}