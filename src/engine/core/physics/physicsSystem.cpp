#include "pch.h"
#include "physicsSystem.h"
#include "..\..\..\common.h"

#define GRAVITY_ACCELERATION -9.81f

static btBroadphaseInterface* broadphase;
static btCollisionDispatcher* dispatcher;
static btDefaultCollisionConfiguration* collisionConfiguration;
static btSequentialImpulseConstraintSolver* solver;

static btDiscreteDynamicsWorld* world;

void physics::init( void )
{
	// Build the broadphase
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher( collisionConfiguration );

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver();

	// The world.
	world = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver , collisionConfiguration );

	world->setGravity(btVector3(0, GRAVITY_ACCELERATION, 0));
}

void physics::destroy( void )
{
	delete world;
	
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
}

void physics::addRigidBody(btRigidBody* body)
{
	world->addRigidBody(body);
}

void physics::update(double dt)
{
	world->stepSimulation( dt*MICROSECONDS_TO_SECONDS_FACTOR,1 );
}
