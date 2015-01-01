#ifndef engine_h__
#define engine_h__

//
//  YorkshireTea
//
//  Created by Karl Jacques on 26/05/2014.
//
//

#include "core/graphics/renderSystem.h"
#include "core/input/inputSystem.h"
#include "core/event/eventSystem.h"
#include "core/networking/clientNetworkSystem.h"
#include "core/networking/serverNetworkSystem.h"
#include "core/physics/physicsManager.h"

#include "core/component/base/entity.h"
#include "engine/core/graphics/UI/ogreConsole.h"


class Engine : public EventListener
{
public:
        
    Engine();
	~Engine();
        
    void update();
    void handle( Event* e );
        
    bool isShuttingDown();

	// Systems getters
	RenderSystem*		getRenderSystem(){ return this->mRenderSystem; };
	PhysicsManager*     getPhysicsManager(){ return mPhysicsManager; };
	EntityManager*		getEntityManager(){return mEntityManager; };
	NetworkSystem*		getNetworkSystem(){return mNetworkSystem; };

	// System
	Ogre::Timer mTimeSinceLastUpdate;

	// Console functions/Engine manipulation
	void SetAsClient( const char* ip );

private:

    RenderSystem* mRenderSystem;
    InputSystem* mInputSystem;
	NetworkSystem* mNetworkSystem;
    
	EntityManager* mEntityManager;
	PhysicsManager* mPhysicsManager;

    bool m_EngineShutdown;
};

#endif // engine_h__
