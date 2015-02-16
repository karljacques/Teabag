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
	comp->sceneNode = render::getRootSceneNode()->createChildSceneNode();
}

void RenderManager::setAsBox( RenderComponent* comp, float3 dim, std::string material )

{
	comp->object = render::getSceneMgr()->createEntity( Ogre::SceneManager::PT_CUBE );
	comp->object->setMaterialName(material);
	
	comp->sceneNode->attachObject(comp->object);
	comp->sceneNode->scale( dim/100.0 );
}

void RenderManager::setAsSphere(RenderComponent* sphere, float radius, std::string material )
{
	Ogre::Entity* ent = render::getSceneMgr()->createEntity( Ogre::SceneManager::PT_SPHERE );
	ent->setMaterialName(material);
	sphere->object = ent;

	sphere->sceneNode->attachObject(sphere->object);
	sphere->sceneNode->scale( float3( radius/100.0 ) );
}

void RenderManager::handle( Event* e )
{
	switch ( e->getEventType() )
	{
	case EV_CORE_SET_TRANSFORM:

		// Get the component the event refers to
		if( componentExists( e->ID ) )
		{
			RenderComponent* rend = getComponentByID<RenderComponent>(e->ID);

			// Get the TransfromEvent
			TransformEvent* trans = e->getData<TransformEvent>();

			// Update internals.
			rend->sceneNode->setPosition(trans->position);
			rend->sceneNode->setOrientation(trans->orientation);
		}

		break;
	}
}

void RenderManager::update( double dt )
{
	for( auto i=mComponents.begin(); i!=mComponents.end();i++ )
	{
		// Has this entity got a TransformComponent? If so, update this component using new transform.
		if( entitysys::getByID(i->second->ID)->hasComponent<TransformComponent>())
		{
			TransformComponent* trans = entitysys::getByID(i->second->ID)->getComponent<TransformComponent>();

			// Get RenderComponent 
			RenderComponent* rend = dynamic_cast<RenderComponent*>(i->second);
			assert(rend!=nullptr);

			// Update position
			rend->sceneNode->setPosition( trans->position );
			rend->sceneNode->setOrientation(trans->orientation);
		}
	}
}

void RenderManager::deinitComponent(Component* comp)
{
	// Cast to render component
	RenderComponent* rend = static_cast<RenderComponent*>(comp);
	render::getSceneMgr()->destroyEntity(rend->object);
	render::getSceneMgr()->destroySceneNode(rend->sceneNode);
}

