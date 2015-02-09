#ifndef manager_h__
#define manager_h__

class Manager 
{
public:
	Manager(){};
	virtual ~Manager(){};

	virtual void update( double dt ) = 0;

private:
};
#endif // manager_h__
