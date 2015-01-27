template <class T> 
class Singleton
{
	public:
		Singleton<T>(){};

	static T* get()
	{
		static T instance;
		return &instance;
	}


};