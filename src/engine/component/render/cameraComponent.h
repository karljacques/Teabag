#ifndef cameraComponent_h__
#define cameraComponent_h__

#include "engine/component/component.h"

class CameraComponent :public Component
{
public:

	Ogre::Camera* camera;
	Ogre::Viewport* mViewport;
	Ogre::SceneNode* mSceneNode;

	Component* clone()
	{
		// Cloning a camera component probably isn't the best idea...
		assert( "Camera is unclonable. Reconsider design.");
		return new CameraComponent();
	}
};

#endif // cameraComponent_h__
