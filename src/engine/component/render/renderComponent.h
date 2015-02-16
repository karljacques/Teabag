#ifndef renderComponent_h__
#define renderComponent_h__

#include "pch.h"
#include "engine/component/component.h"

#include "engine/core/renderSystem.h"
#include "engine/core/entitySystem.h"

class RenderComponent : public Component
{
public:

	Ogre::Entity* object;
	Ogre::SceneNode*	 sceneNode;

	Component* clone( EntID ID )
	{
		RenderComponent* comp = entitysys::createComponent<RenderComponent>(ID);
		comp->object = object->clone(render::generateName());
		comp->sceneNode = render::getRootSceneNode()->createChildSceneNode();
		comp->sceneNode->attachObject(comp->object);
		comp->sceneNode->scale( sceneNode->getScale() );
		return comp;
	}

	void activate()
	{
		this->sceneNode->setVisible(true);
	}

	void deactivate()
	{
		this->sceneNode->setVisible(false);
	}
};

#endif // renderComponent_h__
