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


	boxShape->calculateLocalInertia( 1, btVector3(0,0,0));

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI( 1, 
		new btDefaultMotionState( btTransform( btQuaternion(0,0,0,1), btVector3(0,50,0)))
		, boxShape, btVector3(0,0,0));
	mBody = new btRigidBody(fallRigidBodyCI);
	mPhysicsManager->getDiscreteDynamicsWorld()->addRigidBody(mBody);
}

void PhysicsComponent::update(  double dt  )
{
	//TODO Updates entity data here
	mParent->mPosition = Ogre::Vector3( mBody->getWorldTransform().getOrigin().getX(),mBody->getWorldTransform().getOrigin().getY(),mBody->getWorldTransform().getOrigin().getZ() );
}