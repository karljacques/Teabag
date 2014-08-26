#include "pch.h"
#include "renderComponent.h"
#include "engine/core/component/base/entity.h"


RenderComponent::RenderComponent( RenderSystem* renderSystem, Entity* ent )
{
	mParent = ent;
	mRenderSystem = renderSystem;
	mSceneNode = renderSystem->getRootSceneNode()->createChildSceneNode();
}


RenderComponent::~RenderComponent(void)
{
}

void RenderComponent::update( double dt )
{
}

void RenderComponent::setAsBox( float x, float y, float z )
{
	Ogre::ManualObject* cube = new Ogre::ManualObject("Cube");
	cube->begin("BaseWhiteNoLighting");
	
	x/=2;
	y/=2;
	z/=2;

	// Random-ish colours
	cube->position( x,y,-z ); // v0
	cube->colour( x*2.3,y*1.2,z*3.2 );
	cube->position(-x,y,-z ); // v1 
	cube->colour( x*2.3,y*1.2,z*3.2 );
	cube->position(-x,-y,-z); // v2
	cube->colour( x*2.3,y*1.2,z*3.2 );
	cube->position( x,-y,-z); // v3 
	cube->colour( x*2.3,y*1.2,z*3.2 );
	cube->position( x,-y,z ); // v4
	cube->colour( x*2.3,y*1.2,z*3.2 );
	cube->position( x,y,z  ); // v5
	cube->colour( x*2.3,y*1.2,z*3.2 );
	cube->position( -x,y,z ); // v6
	cube->colour( x*2.3,y*1.2,z*3.2 );
	cube->position( -x,-y,z); // v7
	cube->colour( x*2.3,y*1.2,z*3.2 );

	cube->quad( 0,1,2,3 );
	cube->quad( 5,6,1,0 );
	cube->quad( 6,5,4,7 );
	cube->quad( 1,6,7,2 );
	cube->quad( 5,0,3,4 );
	cube->quad( 3,2,7,4 );

	cube->end(); 

	this->mObject = cube;
	this->mSceneNode->attachObject(mObject);
}

void RenderComponent::handle(  Event* e  )
{
	if( e->getEventType() == EV_Movement )
	{
		MovementEvent* me = static_cast<MovementEvent*>(e);
		this->mSceneNode->setPosition( me->mPosition );
		this->mSceneNode->setOrientation( me->mOrientation );
	}
}