#ifndef componentManager_h__
#define componentManager_h__

#include <unordered_map>

typedef unsigned int EntID;

template <class T>
class ComponentManager
{
public:
	ComponentManager( )
	{
	}
	
	T* createComponent( EntID ID )
	{
		T* comp = new T;
		mComponents[ID] = comp;
		comp->ID = ID;
		return comp;
	}

	void removeComponent( EntID ID )
	{
		mComponents.erase( ID );
	}

	T* getComponentByID( EntID ID )
	{
		try{
			return mComponents.at(ID);
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
	std::unordered_map<EntID, T*> mComponents;

};
#endif // componentManager_h__
