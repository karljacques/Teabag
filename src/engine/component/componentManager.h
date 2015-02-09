#ifndef componentManager_h__
#define componentManager_h__

#include "common.h"
#include "engine/component/component.h"

class ComponentManager
{
public:
	ComponentManager( )
	{
	}

	void add( Component* comp )
	{
		mComponents[comp->ID] = comp;
	}

	void removeComponent( EntID ID )
	{
		mComponents.erase( ID );
	}

	template <class T>
	T* getComponentByID( EntID ID )
	{
		try{
			assert( typeid( *mComponents.at(ID) ) == typeid( T ) );
			return dynamic_cast<T*>(mComponents.at(ID));
		}
		catch(std::out_of_range)
		{
			assert ("Should be checking for existance of component first");
			return nullptr;
		}
	}

	bool componentExists( EntID ID )
	{
		return mComponents.count(ID) > 0;
	}



protected:
	std::unordered_map<EntID, Component*> mComponents;

};
#endif // componentManager_h__
