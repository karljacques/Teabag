#include "pch.h"
#include "cameraManager.h"
#include "..\event\eventSystem.h"

CameraManager::CameraManager(RenderSystem* rend)
{
	mRenderSystem = rend;
}

CameraManager::~CameraManager()
{

}

void CameraManager::createNewCamera(CameraComponent* comp)
{
	// Create camera and viewport
	Ogre::Camera* camera = mRenderSystem->getSceneMgr()->createCamera( mRenderSystem->generateName() );
	camera->setNearClipDistance(1.0f);
	camera->setFarClipDistance(2000.0f);

	Ogre::Viewport* viewport = mRenderSystem->getRenderWindow()->addViewport( camera );
	viewport->setBackgroundColour( Ogre::ColourValue(0.0f,0.0f,0.5f));

	camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()) );

	// Attach to scenenode
	comp->mSceneNode = mRenderSystem->getRootSceneNode()->createChildSceneNode();
	comp->mSceneNode->attachObject( camera );

	// Register camera pair with rendersystem
	mRenderSystem->addCameraPair( camera, viewport );
}

void CameraManager::setCameraOffset(CameraComponent* comp, float3 offset)
{
	comp->mCamera->setPosition(offset);
}

void CameraManager::lookAt(CameraComponent* comp, float3 pos)
{
	comp->mCamera->lookAt( pos );

	Event* e = EventSystem::getSingletonPtr()->getEvent( EV_CORE_TRANSFORM_UPDATE );
	TransformEvent* me = e->createEventData<TransformEvent>();
	me->mFloat3_1 = comp->mSceneNode->getPosition();
	me->mQuaternion = comp->mCamera->getDerivedOrientation();

	EventSystem::getSingletonPtr()->dispatchEvent(e);

}

void CameraManager::handle(Event* e)
{
	// Get Component
	CameraComponent* comp = getComponentByLUID(e->LUID);

	if( comp )
	{
		if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
		{
			TransformEvent* me = e->getData<TransformEvent>();
			comp->mSceneNode->setPosition( me->mFloat3_1 );
			comp->mCamera->setOrientation( me->mQuaternion );
		}
	}
}
