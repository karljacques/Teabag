#ifndef renderComponent_h__
#define renderComponent_h__

#include "engine/core/component/base/component.h"
#include "common.h"

class RenderComponent : public Component
{
public:

	Ogre::MovableObject* mObject;
	Ogre::SceneNode*	 mSceneNode;

};

#endif // renderComponent_h__
