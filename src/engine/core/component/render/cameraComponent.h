#pragma once

#include "engine\core\component\base\component.h"
#include "engine\core\component\base\positionComponent.h"
#include "engine\core\graphics\RenderSystem.h"
#include "engine\core\event\EventListener.h"

// dependent on Ogre
class CameraComponent :
	public Component, public EventListener
{
public:
	CameraComponent( RenderSystem* renderSystem, PositionComponent* positionComponent );
	~CameraComponent(void);

	void handle(  Event* e );

	// Don't want to put these in position component
	void setPosition( float3 pos );
	void lookAt( float3 pos );
	void update(  double dt  );

	// Little hack function
	void setOffset( float3 offset );

private:

	RenderSystem* mRenderSystem;
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	Ogre::SceneNode* mSceneNode;

	PositionComponent* mPositionComponent;
};

