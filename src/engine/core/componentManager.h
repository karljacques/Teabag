#include "../world/entityManager.h"

template <class T>
class ComponentManager
{
public:
	ComponentManager( EntityManager* ent )
	{
		mEntityMgr = ent;
		mNetworkSys = net;
	}
	
	T* createComponent( );
	void removeComponent( T* comp );

	T* getComponentByGUID( int guid );

protected:
	std::vector<T*> mComponents;
	EntityManager* mEntityMgr;
	NetworkSystem* mNetworkSys;

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

template <class T>
T* ComponentManager::getComponentByGUID( int guid )
{
	for( auto i = mComponents.begin(); i != mComponents.end(); i++ )
	{
		if( (*i)->GUID == GUID )
			return (*i);
	}
	return nullptr;
}