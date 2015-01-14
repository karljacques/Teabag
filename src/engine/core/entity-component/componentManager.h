#ifndef componentManager_h__
#define componentManager_h__

template <class T>
class ComponentManager
{
public:
	ComponentManager( )
	{
	}
	
	T* createComponent( int ID )
	{
		T* comp = new T;
		mComponents[ID] = comp;
		comp->ID = ID;
		return comp;
	}

	void removeComponent( int ID )
	{
		mComponents.erase( ID );
	}

	T* getComponentByID( int ID )
	{
		try{
			return mComponents.at(ID);
		}
		catch(std::out_of_range)
		{
			return nullptr;
		}
	}

	bool componentExists( int ID )
	{
		return mComponents.count(ID) > 0;
	}

protected:
	std::unordered_map<int, T*> mComponents;

};
#endif // componentManager_h__
