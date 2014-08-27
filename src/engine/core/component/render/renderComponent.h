#ifndef renderComponent_h__
#define renderComponent_h__

#include "engine\core\component\base\component.h"
#include "engine\core\component\base\positionComponent.h"
#include "engine\core\graphics\RenderSystem.h"
#include "engine\core\event\EventListener.h"
#include "common.h"

class RenderComponent : public Component, public EventListener
{
public:
	RenderComponent( RenderSystem* renderSystem, PositionComponent* positionComponent );
	~RenderComponent(void);

	void update( double dt );
	void setAsBox( float x,float y,float z );

	void handle( Event* e );

private:
	RenderSystem*		 mRenderSystem;

	Ogre::MovableObject* mObject;
	Ogre::SceneNode*	 mSceneNode;

	PositionComponent* mPositionComponent;
};

#endif // renderComponent_h__
