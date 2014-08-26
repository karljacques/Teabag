#pragma once

#include "engine\core\component\base\component.h"
#include "engine\core\graphics\RenderSystem.h"
#include "engine\core\event\EventListener.h"

// dependent on Ogre
class CameraComponent :
	public Component, public EventListener
{
public:
	CameraComponent( RenderSystem* renderSystem );
	~CameraComponent(void);

	void handle(  Event* e );

	// Don't want to put these in position component
	void setPosition( float3 pos );
	void lookAt( float3 pos );

private:

	RenderSystem* mRenderSystem;
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	Ogre::SceneNode* mSceneNode;
};

