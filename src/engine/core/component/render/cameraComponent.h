#pragma once

#include "engine/core/component/base/component.h"
#include "engine/core/component/base/positionComponent.h"
#include "engine/core/graphics/renderSystem.h"
#include "engine/core/event/eventListener.h"

class CameraComponent :public Component
{
public:
	CameraComponent(  );
	~CameraComponent(void);

	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	Ogre::SceneNode* mSceneNode;

};

