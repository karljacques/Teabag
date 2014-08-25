#ifndef physicsManager_h__
#define physicsManager_h__

#include <btBulletDynamicsCommon.h>

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void update( double dt );

	// Getters
	btDiscreteDynamicsWorld* getDiscreteDynamicsWorld(){ return mWorld; };

private:
	btDiscreteDynamicsWorld* mWorld;
};
#endif // physicsManager_h__