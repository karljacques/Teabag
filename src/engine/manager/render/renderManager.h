#ifndef  __Teabag__Rendersystem__
#define __Teabag__Rendersystem__

#include "pch.h"
#include "../../core/entity-component/componentManager.h"
#include "../../component/render/renderComponent.h"
#include "../../core/entity-component/entityManager.h"
#include "../../component/physics/physicsComponent.h"
#include "../manager.h"

class RenderManager : public ComponentManager, public EventListener, public Manager
{
public:
        
    RenderManager( );
    ~RenderManager();
	
	// Component Methods
	void initComponent( RenderComponent* comp );
	void setAsBox( RenderComponent* comp, float3 dim );

	void handle( Event* e );
	void update( double dt );

private:
        


};

#endif /* defined( __Teabag__Rendersystem__) */
