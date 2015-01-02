#ifndef cameraComponent_h__
#define cameraComponent_h__

#include "../base/component.h"

class CameraComponent :public Component
{
public:

	Ogre::Camera* camera;
	Ogre::Viewport* mViewport;
	Ogre::SceneNode* mSceneNode;

};

#endif // cameraComponent_h__
