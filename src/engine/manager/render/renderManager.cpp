//
//  gfxSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//

#include "pch.h"
#include "renderManager.h"
#include <SDL.h>
#include <SDL_syswm.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "../../core/render/OSXUtils.h"
#endif
#include "../../core/render/renderSystem.h"

RenderManager::RenderManager( )
{  
}

RenderManager::~RenderManager()
{
}

void RenderManager::initComponent( RenderComponent* comp )
{
	comp->mSceneNode = render::getRootSceneNode()->createChildSceneNode();
}

void RenderManager::setAsBox( RenderComponent* comp, float3 dim )

{
	Ogre::ManualObject* cube = new Ogre::ManualObject("Cube");
	cube->begin("BaseWhiteNoLighting");

	float x = dim.x/2.0;
	float y = dim.y/2.0;
	float z = dim.z/2.0;

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

	comp->mObject = cube;
	comp->mSceneNode->attachObject(comp->mObject);
}

void RenderManager::handle( Event* e )
{
	if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
	{
		if( componentExists(e->ID ))
		{
			RenderComponent* comp = getComponentByID<RenderComponent>( e->ID );
			TransformEvent* te = e->getData<TransformEvent>();
			comp->mSceneNode->setPosition( te->position );
			comp->mSceneNode->setOrientation( te->orientation );
		}

	}
}

void RenderManager::update( double dt )
{

}