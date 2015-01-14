
#ifndef engine_h__
#define engine_h__

/*
//  Teabag
//
//  Created as 'Yorkshire Tea' by Karl Jacques on 26/05/2014.
//
//
*/

#include "manager/render/renderSystem.h"
#include "manager/physics/physicsManager.h"
#include "core/entity-component/entityManager.h"
#include "manager/network/networkSystem.h"
#include "manager/render/cameraManager.h"
#include "core/input/inputSystem.h"

#include "core/user-interface/ogreConsole.h"
#include "manager/abstract/spectatorManager.h"
#include "core/user-interface/debugDisplaySystem.h"



class InputSystem;

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
	CameraManager*		getCameraManager(){return mCameraManager; };
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
	CameraManager* mCameraManager;

	SpectatorManager* mSpectatorManager;

	DebugDisplaySystem* mDebugDisplaySystem;


    bool m_EngineShutdown;

};

#endif // engine_h__
