#include "pch.h"
#include "physicsComponent.h"
#include "entity.h"

PhysicsComponent::PhysicsComponent( PhysicsManager* physicsManager, Entity* ent )
{
	mPhysicsManager = physicsManager;
	mParent = ent;
}


PhysicsComponent::~PhysicsComponent(void)
{
}

void PhysicsComponent::setAsBox( float x, float y, float z )
{
	btCollisionShape* boxShape = new  btBoxShape( btVector3( x,y,z ));

	btVector3 nullVector(0,0,0);
	boxShape->calculateLocalInertia( 1, nullVector);

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( 1,
		new btDefaultMotionState( btTransform( btQuaternion(0,0,0,1), btVector3(0,50,0)))
		, boxShape, nullVector);
	mBody = new btRigidBody(fallRigidBodyCI);
	mPhysicsManager->getDiscreteDynamicsWorld()->addRigidBody(mBody);
}

void PhysicsComponent::update(  double dt  )
{
	//TODO Updates entity data here
	//mParent->mPosition = Ogre::Vector3( mBody->getWorldTransform().getOrigin().getX(),mBody->getWorldTransform().getOrigin().getY(),mBody->getWorldTransform().getOrigin().getZ() );

	MovementEvent* e = new MovementEvent(EV_Movement);
	e->x = mBody->getWorldTransform().getOrigin().getX();
	e->y = mBody->getWorldTransform().getOrigin().getY();
	e->z = mBody->getWorldTransform().getOrigin().getZ();

	dispatch( e );
}
