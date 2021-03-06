#ifndef  __Teabag__Rendersystem__
#define __Teabag__Rendersystem__

#include "pch.h"

#include "renderComponent.h"
#include "engine/component/physics/transformComponent.h"

#include "engine/component/componentManager.h"
#include "engine/manager.h"

#include "engine/core/renderSystem.h"
#include "engine/core/eventSystem.h"
#include "engine/core/entitySystem.h"

#include "engine/core/event/events/transfromEvent.h"

class RenderManager : public ComponentManager, public EventListener, public Manager
{
public:
        
    RenderManager( );
    ~RenderManager();
	
	// Component Methods
	void initComponent( RenderComponent* comp );
	void deinitComponent( Component* comp );

	void setAsBox( RenderComponent* comp, float3 dim, std::string material = "SimpleCrate/Textured" );

	void handle( Event* e );
	void update( double dt );

private:
        


};

#endif /* defined( __Teabag__Rendersystem__) */
