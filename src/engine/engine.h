//
//  YorkshireTea
//
//  Created by Karl Jacques on 26/05/2014.
//
//

#include "core/graphics/RenderSystem.h"
#include "core/input/inputSystem.h"
#include "core/event/eventSystem.h"
#include "core/component/base/entity.h"
#include "core/physics/physicsManager.h"
#include "terrain/terrain.h"
#include "engine/world/staticGeometry.h"

class Engine : public EventListener
{
public:
        
    Engine();
	~Engine();
        
    void update();
    void handle( Event* e );
        
    bool isShuttingDown();
        
    // Camera commands
    Camera*         createCamera( );
        
    // Terrain commands
    Terrain*        createTerrain();

	// Systems getters
	RenderSystem*		getRenderSystem(){ return this->mRenderSystem; };
	PhysicsManager*     getPhysicsManager(){ return mPhysicsManager; };
       
	// System
	Ogre::Timer mTimeSinceLastUpdate;

	// Entities
	std::vector<Entity*> mEntities;
	Entity* createEntity();
	void spawnNewCube();

private:
    RenderSystem* mRenderSystem;
    InputSystem* mInputSystem;
    EventSystem* mEventSystem;
    
	PhysicsManager* mPhysicsManager;

	StaticGeometry* mStaticGeometry;

    bool m_EngineShutdown;

};

