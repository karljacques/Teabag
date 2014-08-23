#ifndef renderComponent_h__
#define renderComponent_h__

#include "component.h"
#include "engine/core/graphics/RenderSystem.h"

class RenderComponent : public Component
{
public:
	RenderComponent( RenderSystem* renderSystem, Entity* ent );
	~RenderComponent(void);

	RenderSystem*		 mRenderSystem;

	Ogre::MovableObject* mObject;
	Ogre::SceneNode*	 mSceneNode;

	void update( double dt );
	void setAsBox( float x,float y,float z );
};

#endif // renderComponent_h__
