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
	viewport->setBackgroundColour( Ogre::ColourValue(0.0f,0.0f,0.5f));

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

	Event* e = eventsys::get( EV_CORE_TRANSFORM_UPDATE_ORIENTATION, comp->ID,this );
	TransformEvent* me = e->createEventData<TransformEvent>();
	me->orientation = comp->camera->getDerivedOrientation();

	eventsys::dispatch(e);
	
}

void CameraManager::handle(Event* e)
{
	// Get Component
	if( this->componentExists(e->ID) )
	{
		CameraComponent* comp = getComponentByID<CameraComponent>(e->ID);
		if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
		{
			TransformEvent* me = e->getData<TransformEvent>();
			comp->mSceneNode->setPosition( me->position );
			comp->camera->setOrientation( me->orientation );
		}
	}
	
}

Ogre::Viewport* CameraManager::getViewport()
{
	return m_CameraMap.begin()->second;
}


void CameraManager::update( double dt )
{

}

void CameraManager::getCamera(CameraComponent* comp)
{
	comp->camera = m_CameraMap.begin()->first;
	comp->mViewport = m_CameraMap.begin()->second;

	comp->mSceneNode = m_CameraMap.begin()->first->getParentSceneNode();
}