#ifndef componentManager_h__
#define componentManager_h__

#include "../world/entityManager.h"

template <class T>
class ComponentManager
{
public:
	ComponentManager( EntityManager* ent )
	{
		mEntityMgr = ent;
	}
	
	T* ComponentManager<T>::createComponent()
	{
		T* comp = new T;
		mComponents.push_back(comp);
		return comp;
	}

	void ComponentManager<T>::removeComponent( T* comp )
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

	T* ComponentManager<T>::getComponentByGUID( int guid )
	{
		for( auto i = mComponents.begin(); i != mComponents.end(); i++ )
		{
			if( (*i)->GUID == guid )
				return (*i);
		}
		return nullptr;
	}

protected:
	std::vector<T*> mComponents;
	EntityManager* mEntityMgr;

};
#endif // componentManager_h__
