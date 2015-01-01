#ifndef componentManager_h__
#define componentManager_h__

template <class T>
class ComponentManager
{
public:
	ComponentManager( )
	{
	}
	
	T* ComponentManager<T>::createComponent( int LUID )
	{
		T* comp = new T;
		mComponents[LUID] = comp;
		comp->LUID = LUID;
		return comp;
	}

	void ComponentManager<T>::removeComponent( int LUID )
	{
		mComponents.erase( LUID );
	}

	T* ComponentManager<T>::getComponentByLUID( int LUID )
	{
		try{
			return mComponents.at(LUID);
		}
		catch(std::out_of_range)
		{
			return nullptr;
		}
	}

	bool ComponentManager<T>::componentExists( int LUID )
	{
		return mComponents.count(LUID) > 0;
	}

protected:
	std::unordered_map<int, T*> mComponents;

};
#endif // componentManager_h__
