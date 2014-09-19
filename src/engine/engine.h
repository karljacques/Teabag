#ifndef engine_h__
#define engine_h__

//
//  YorkshireTea
//
//  Created by Karl Jacques on 26/05/2014.
//
//

#include "core/graphics/RenderSystem.h"
#include "core/input/inputSystem.h"
#include "core/event/eventSystem.h"
#include "core/networking/clientNetworkSystem.h"
#include "core/networking/serverNetworkSystem.h"
#include "core/physics/physicsManager.h"

#include "core/component/base/entity.h"

#include "engine/world/staticGeometry.h"

#include "engine/core/graphics/UI/OgreConsole.h"

class Engine : public EventListener
{
public:
        
    Engine();
	~Engine();
        
    void update();
    void handle( Event* e );
        
    bool isShuttingDown();
        
    // Terrain commands

	// Systems getters
	RenderSystem*		getRenderSystem(){ return this->mRenderSystem; };
	PhysicsManager*     getPhysicsManager(){ return mPhysicsManager; };
	EventSystem*		getEventSystem(){return mEventSystem; };
       
	// System
	Ogre::Timer mTimeSinceLastUpdate;

	// Entities
	std::vector<Entity*> mEntities;
	Entity* createEntity();

	// Console functions/Engine manipulation
	void SetAsClient( const char* ip );

private:

    RenderSystem* mRenderSystem;
    InputSystem* mInputSystem;
    EventSystem* mEventSystem;
	NetworkSystem* mNetworkSystem;
    
	PhysicsManager* mPhysicsManager;

	StaticGeometry* mStaticGeometry;

    bool m_EngineShutdown;


};

#endif // engine_h__
