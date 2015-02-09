#ifndef renderComponent_h__
#define renderComponent_h__

#include "pch.h"
#include "engine/component/component.h"

#include "engine/core/renderSystem.h"

class RenderComponent : public Component
{
public:

	Ogre::Entity* mObject;
	Ogre::SceneNode*	 mSceneNode;

	Component* clone()
	{
		RenderComponent* comp = new RenderComponent();
		comp->mObject = mObject->clone(render::generateName());
		return comp;
	}
};

#endif // renderComponent_h__
