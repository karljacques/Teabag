#include "pch.h"
#include "common.h"

#ifndef physicsSystem_h__
#define physicsSystem_h__

namespace physics
{
	void init( void );
	void destroy( void );

	void addRigidBody( btRigidBody* body );

	void update( double dt );
}


#endif // physicsSystem_h__