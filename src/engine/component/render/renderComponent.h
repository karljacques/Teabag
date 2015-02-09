#ifndef renderComponent_h__
#define renderComponent_h__

#include "pch.h"
#include "engine/component/component.h"

#include "engine/core/renderSystem.h"
#include "engine/core/entitySystem.h"

class RenderComponent : public Component
{
public:

	Ogre::Entity* mObject;
	Ogre::SceneNode*	 mSceneNode;

	Component* clone( EntID ID )
	{
		RenderComponent* comp = entitysys::createComponent<RenderComponent>(ID);
		comp->mObject = mObject->clone(render::generateName());
		comp->mSceneNode = render::getRootSceneNode()->createChildSceneNode();
		comp->mSceneNode->attachObject(comp->mObject);
		comp->mSceneNode->scale( mSceneNode->getScale() );
		return comp;
	}

	void activate()
	{
		this->mSceneNode->setVisible(true);
	}

	void deactivate()
	{
		this->mSceneNode->setVisible(false);
	}
};

#endif // renderComponent_h__
