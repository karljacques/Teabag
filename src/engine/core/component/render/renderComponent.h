#ifndef renderComponent_h__
#define renderComponent_h__

#include "engine/core/component/base/component.h"
#include "engine/core/component/base/positionComponent.h"
#include "engine/core/graphics/renderSystem.h"
#include "engine/core/event/eventListener.h"
#include "common.h"

class RenderComponent : public Component
{
public:
	RenderComponent( );
	~RenderComponent(void);


	Ogre::MovableObject* mObject;
	Ogre::SceneNode*	 mSceneNode;

	PositionComponent* mPositionComponent;
};

#endif // renderComponent_h__
