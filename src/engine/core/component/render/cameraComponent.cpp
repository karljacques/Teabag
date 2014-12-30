#include "pch.h"
#include "cameraComponent.h"
#include "engine/core/event/eventSystem.h"

CameraComponent::CameraComponent(RenderSystem* renderSystem, PositionComponent* positionComponent ) 
{
	mRenderSystem = renderSystem;
	mPositionComponent = positionComponent;

	// Create camera and viewport
	mCamera = renderSystem->getSceneMgr()->createCamera( renderSystem->generateName() );
	mCamera->setNearClipDistance(1.0f);
	mCamera->setFarClipDistance(2000.0f);

	mViewport = renderSystem->getRenderWindow()->addViewport( mCamera );
	mViewport->setBackgroundColour( Ogre::ColourValue(0.0f,0.0f,0.5f));

	mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()) );
	
	// Attach to scenenode
	mSceneNode = renderSystem->getRootSceneNode()->createChildSceneNode();
	mSceneNode->attachObject( mCamera );

	// Register camera pair with rendersystem
	renderSystem->addCameraPair( mCamera, mViewport );
}


CameraComponent::~CameraComponent(void)
{
}

void CameraComponent::handle( Event* e )
{
	if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
	{
		TransformEvent* me = e->getData<TransformEvent>();
		mSceneNode->setPosition( me->mFloat3_1 );
		mCamera->setOrientation( me->mQuaternion );
	}
}

void CameraComponent::setPosition( float3 pos )
{
	Event* e = EventSystem::getSingletonPtr()->getEvent( EV_CORE_TRANSFORM_UPDATE );
	TransformEvent* me = e->createEventData<TransformEvent>();
	me->mFloat3_1 = pos;
	me->mQuaternion = mSceneNode->getOrientation();

	dispatch(e);

	mSceneNode->setPosition(pos);
}

void CameraComponent::lookAt( float3 pos )
{
	mCamera->lookAt( pos );

	Event* e = EventSystem::getSingletonPtr()->getEvent( EV_CORE_TRANSFORM_UPDATE );
	TransformEvent* me = e->createEventData<TransformEvent>();
	me->mFloat3_1 = mSceneNode->getPosition();
	me->mQuaternion = mCamera->getDerivedOrientation();

	dispatch( e );
}

void CameraComponent::setOffset( float3 offset )
{
	mCamera->setPosition( offset );
}

void CameraComponent::update( double dt )
{
	//mCamera->lookAt( mSceneNode->getPosition() );
	mSceneNode->setPosition( mPositionComponent->getPosition() );
	mCamera->setOrientation( mPositionComponent->getOrientation() );
}