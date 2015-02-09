#ifndef cameraComponent_h__
#define cameraComponent_h__

#include "../base/component.h"

class CameraComponent :public Component
{
public:

	Ogre::Camera* camera;
	Ogre::Viewport* mViewport;
	Ogre::SceneNode* mSceneNode;

	Component* clone()
	{
		return new CameraComponent();
	}
};

#endif // cameraComponent_h__
