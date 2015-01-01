#ifndef cameraComponent_h__
#define cameraComponent_h__

#include "engine/core/component/base/component.h"
#include "engine/core/graphics/renderSystem.h"

class CameraComponent :public Component
{
public:

	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	Ogre::SceneNode* mSceneNode;

};

#endif // cameraComponent_h__
