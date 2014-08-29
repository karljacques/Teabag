#include "pch.h"
#include "physicsManager.h"

PhysicsManager::PhysicsManager()
{
		
		// Build the broadphase
		btBroadphaseInterface* broadphase = new btDbvtBroadphase();

		// Set up the collision configuration and dispatcher
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

		// The actual physics solver
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

		// The world.
		mWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		mWorld->setGravity(btVector3(0, -9.81f, 0));



}

PhysicsManager::~PhysicsManager()
{
	delete mWorld;
}

void PhysicsManager::update( double dt )
{
	mWorld->stepSimulation( 1.0/60.0f ,8 );
}

