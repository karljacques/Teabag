#include "pch.h"
#include "..\..\core\event\eventSystem.h"


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
	comp->camera = mRenderSystem->getSceneMgr()->createCamera( mRenderSystem->generateName() );
	comp->camera->setNearClipDistance(1.0f);
	comp->camera->setFarClipDistance(2000.0f);

	Ogre::Viewport* viewport = mRenderSystem->getRenderWindow()->addViewport( comp->camera );
	viewport->setBackgroundColour( Ogre::ColourValue(0.0f,0.0f,0.5f));

	comp->camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()) );

	// Attach to scenenode
	comp->mSceneNode = mRenderSystem->getRootSceneNode()->createChildSceneNode();
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

	Event* e = EventSystem::getSingletonPtr()->getEvent( EV_CORE_TRANSFORM_UPDATE_ORIENTATION, comp->ID,this );
	TransformEvent* me = e->createEventData<TransformEvent>();
	me->mQuaternion = comp->camera->getDerivedOrientation();

	EventSystem::getSingletonPtr()->dispatchEvent(e);
	
}

void CameraManager::handle(Event* e)
{
	// Get Component
	CameraComponent* comp = getComponentByID(e->ID);

	if( comp )
	{
		if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
		{
			TransformEvent* me = e->getData<TransformEvent>();
			comp->mSceneNode->setPosition( me->mFloat3_1 );
			comp->camera->setOrientation( me->mQuaternion );
		}
	}
}

Ogre::Viewport* CameraManager::getViewport()
{
	return m_CameraMap.begin()->second;
}
