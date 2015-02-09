#ifndef renderComponent_h__
#define renderComponent_h__

#include "pch.h"
#include "engine/component/component.h"

class RenderComponent : public Component
{
public:

	Ogre::MovableObject* mObject;
	Ogre::SceneNode*	 mSceneNode;

	Component* clone()
	{
		return new RenderComponent();
	}
};

#endif // renderComponent_h__
