//
//  gfxSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//

#include "pch.h"
#include "renderManager.h"


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "engine/core/render/OSXUtils.h"
#endif


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

void RenderManager::setAsBox( RenderComponent* comp, float3 dim, std::string material )

{
	Ogre::Entity* ent = render::getSceneMgr()->createEntity( Ogre::SceneManager::PT_CUBE );
	ent->setMaterialName(material);
	comp->mObject = ent;
	
	comp->mSceneNode->attachObject(comp->mObject);
	comp->mSceneNode->scale( dim/100.0 );
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

void RenderManager::deinitComponent(Component* comp)
{
	// Cast to render component
	RenderComponent* rend = static_cast<RenderComponent*>(comp);
	render::getSceneMgr()->destroyEntity(rend->mObject);
	render::getSceneMgr()->destroySceneNode(rend->mSceneNode);
}
