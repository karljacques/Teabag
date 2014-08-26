#include "pch.h"
#include "cameraComponent.h"


CameraComponent::CameraComponent(RenderSystem* renderSystem) 
{
	mRenderSystem = renderSystem;

	// Create camera and viewport
	mCamera = renderSystem->getSceneMgr()->createCamera( renderSystem->generateName() );
	mCamera->setNearClipDistance(0.1f);
	mCamera->setFarClipDistance(100.0f);

	mViewport = renderSystem->getRenderWindow()->addViewport( mCamera );
	mViewport->setBackgroundColour( Ogre::ColourValue(0.0f,0.0f,0.5f));

	mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()) );
	
	// Attach to scenenode
	mSceneNode = renderSystem->getRootSceneNode()->createChildSceneNode();
	mSceneNode->attachObject( mCamera );

}


CameraComponent::~CameraComponent(void)
{
}

void CameraComponent::handle( Event* e )
{
	if( e->getEventType() == EV_Movement )
	{
		MovementEvent* me = static_cast<MovementEvent*>(e);
		mSceneNode->setPosition( me->mPosition );
		mSceneNode->setOrientation( me->mOrientation );
	}
}

void CameraComponent::setPosition( float3 pos )
{
	MovementEvent* me = new MovementEvent( EV_Movement );
	me->mPosition = pos;
	me->mOrientation = mSceneNode->getOrientation();

	dispatch(me);

	mSceneNode->setPosition(pos);
}

void CameraComponent::lookAt( float3 pos )
{
	mCamera->lookAt( pos );

	MovementEvent* me = new MovementEvent( EV_Movement );
	me->mPosition = mSceneNode->getPosition();
	me->mOrientation = mCamera->getDerivedOrientation();

	dispatch( me );
}