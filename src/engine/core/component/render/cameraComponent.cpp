#include "pch.h"
#include "cameraComponent.h"


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
		TransformEvent* me = static_cast<TransformEvent*>(e);
		mSceneNode->setPosition( me->mPosition );
		mCamera->setOrientation( me->mOrientation );
	}
}

void CameraComponent::setPosition( float3 pos )
{
	TransformEvent* me = new TransformEvent( EV_CORE_TRANSFORM_UPDATE );
	me->mPosition = pos;
	me->mOrientation = mSceneNode->getOrientation();

	dispatch(me);

	mSceneNode->setPosition(pos);
}

void CameraComponent::lookAt( float3 pos )
{
	mCamera->lookAt( pos );

	TransformEvent* me = new TransformEvent( EV_CORE_TRANSFORM_UPDATE );
	me->mPosition = mSceneNode->getPosition();
	me->mOrientation = mCamera->getDerivedOrientation();

	dispatch( me );
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