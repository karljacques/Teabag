
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
class OgreConsole;

using namespace std;

class Engine : public EventListener
{
public:
        
    Engine();
	~Engine();
        
    void update();
    void handle( Event* e );
        
    bool isShuttingDown();

	// Systems getters
	RenderSystem*		getRenderSystem(){ return mRenderSystem.get(); };
	PhysicsManager*     getPhysicsManager(){ return mPhysicsManager.get(); };
	EntityManager*		getEntityManager(){return mEntityManager.get(); };
	NetworkSystem*		getNetworkSystem(){return mNetworkSystem.get(); };
	CameraManager*		getCameraManager(){return mCameraManager.get(); };
	// System
	Ogre::Timer mTimeSinceLastUpdate;

	// Console functions/Engine manipulation
	void SetAsClient( const char* ip );

private:

    shared_ptr<RenderSystem> mRenderSystem;
    shared_ptr<InputSystem> mInputSystem;
	shared_ptr<NetworkSystem> mNetworkSystem;
    
	shared_ptr<EntityManager> mEntityManager;
	shared_ptr<PhysicsManager> mPhysicsManager;
	shared_ptr<CameraManager> mCameraManager;

	shared_ptr<SpectatorManager> mSpectatorManager;

	//shared_ptr<DebugDisplaySystem> mDebugDisplaySystem;

	shared_ptr<Engine> mSelf;
	shared_ptr<OgreConsole> mConsole;


    bool m_EngineShutdown;

};

#endif // engine_h__
