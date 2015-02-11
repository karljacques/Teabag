#include "pch.h"
#include "cameraManager.h"


CameraManager::CameraManager()
{

}

CameraManager::~CameraManager()
{

}

void CameraManager::createNewCamera(CameraComponent* comp)
{
	// Create camera and viewport
	comp->camera = render::getSceneMgr()->createCamera( render::generateName() );
	comp->camera->setNearClipDistance(0.01f);
	comp->camera->setFarClipDistance(200.0f);

	Ogre::Viewport* viewport = render::getRenderWindow()->addViewport( comp->camera );
	//viewport->setBackgroundColour( Ogre::ColourValue(0.0f,0.0f,0.5f));

	comp->camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()) );

	// Attach to scenenode
	comp->mSceneNode = render::getRootSceneNode()->createChildSceneNode();
	comp->mSceneNode->attachObject( comp->camera );

	// Register camera pair
	m_CameraMap[comp->camera] = viewport;
}

void CameraManager::setCameraOffset(CameraComponent* comp, float3 offset)
{
	comp->camera->setPosition(offset);
}

void CameraManager::lookAt(CameraComponent* comp, float3 pos)
{
	comp->camera->lookAt( pos );

	// If camera has transformComponent it needs to be updated
	if ( entitysys::getByID( comp->ID)->hasComponent<TransformComponent>())
	{
		TransformComponent* trans = entitysys::getByID( comp->ID )->getComponent<TransformComponent>();
		trans->orientation = comp->camera->getDerivedOrientation();
	}
	
}

void CameraManager::handle(Event* e)
{

	
}

Ogre::Viewport* CameraManager::getViewport()
{
	return m_CameraMap.begin()->second;
}


void CameraManager::update( double dt )
{
	for( auto i=mComponents.begin(); i!=mComponents.end(); i++ )
	{
		// Get if camera has TransformComponent (it should, but not strictly necessary I guess)
		if ( entitysys::getByID( i->second->ID)->hasComponent<TransformComponent>())
		{
			TransformComponent* trans = entitysys::getByID( i->second->ID)->getComponent<TransformComponent>();

			// Cast i to a CameraComponent so we can access its variables.
			CameraComponent* cam = dynamic_cast<CameraComponent*>(i->second);
			assert( cam!= nullptr );

			// Update camera transform
			cam->mSceneNode->setPosition( trans->position );
			cam->camera->setOrientation( trans->orientation );
		}
	}
}

void CameraManager::getCamera(CameraComponent* comp)
{
	comp->camera = m_CameraMap.begin()->first;
	comp->mViewport = m_CameraMap.begin()->second;

	comp->mSceneNode = m_CameraMap.begin()->first->getParentSceneNode();
}
