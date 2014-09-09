#include "pch.h"
#include "renderComponent.h"
#include "engine/core/component/base/entity.h"


RenderComponent::RenderComponent( RenderSystem* renderSystem, PositionComponent* positionComponent )
{
	mRenderSystem = renderSystem;
	mSceneNode = renderSystem->getRootSceneNode()->createChildSceneNode();
	mPositionComponent = positionComponent;
}


RenderComponent::~RenderComponent(void)
{
}


void RenderComponent::setAsBox( float x, float y, float z )
{
	Ogre::ManualObject* cube = new Ogre::ManualObject("Cube");
	cube->begin("BaseWhiteNoLighting");
	
	x/=2;
	y/=2;
	z/=2;

	float c1 = rand() %255;
	float c2 = rand() %255;
	float c3 = rand() %255;
	// Random-ish colours
	cube->position( x,y,-z ); // v0
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position(-x,y,-z ); // v1 
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position(-x,-y,-z); // v2
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position( x,-y,-z); // v3 
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position( x,-y,z ); // v4
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position( x,y,z  ); // v5
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position( -x,y,z ); // v6
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position( -x,-y,z); // v7
	cube->colour( x*c1,y*c2,z*c3 );

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

}

void RenderComponent::update( double dt )
{
	mSceneNode->setPosition( mPositionComponent->getPosition() );
	mSceneNode->setOrientation( mPositionComponent->getOrientation() );
}