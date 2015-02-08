
#ifndef engine_h__
#define engine_h__

/*
//  Teabag
//
//  Created as 'Yorkshire Tea' by Karl Jacques on 26/05/2014.
//
//
*/

#include "manager/render/renderManager.h"
#include "manager/physics/physicsManager.h"
#include "core/entity-component/entityManager.h"
#include "manager/network/networkSystem.h"
#include "manager/render/cameraManager.h"
#include "core/input/inputSystem.h"
#include "core/physics/physicsSystem.h"

#include "core/user-interface/ogreConsole.h"
#include "manager/abstract/spectatorManager.h"
#include "core/user-interface/debugDisplaySystem.h"

#include "manager/abstract/playerManager.h"

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

	// Manager methods
	void registerManager( weak_ptr<Manager> mgr );
	void removeManager( weak_ptr<Manager> mgr );

	// Systems getters
	RenderManager*		getRenderSystem(){ return mRenderSystem.get(); };
	PhysicsManager*     getPhysicsManager(){ return mPhysicsManager.get(); };
	EntityManager*		getEntityManager(){return mEntityManager.get(); };
	NetworkComponentManager*		getNetworkSystem(){return mNetworkSystem.get(); };
	CameraManager*		getCameraManager(){return mCameraManager.get(); };
	PlayerManager*		getPlayerManager(){ return mPlayerMgr.get(); };

	// System
	Ogre::Timer mTimeSinceLastUpdate;
	Ogre::Timer mTimeSinceLastRender;

	// Console functions/Engine manipulation
	void SetAsClient( const char* ip );

private:
	
	bool m_EngineShutdown;
	
	shared_ptr<DebugDisplaySystem> mDebugDisplaySystem;
	shared_ptr<Engine> mSelf;
	shared_ptr<PlayerManager> mPlayerMgr;

	shared_ptr<EntityManager> mEntityManager;
	shared_ptr<PhysicsManager> mPhysicsManager;
	shared_ptr<CameraManager> mCameraManager;
	shared_ptr<OgreConsole> mConsole;
	shared_ptr<SpectatorManager> mSpectatorManager;

	shared_ptr<RenderManager> mRenderSystem;
	shared_ptr<InputSystem> mInputSystem;
	shared_ptr<NetworkComponentManager> mNetworkSystem;

	std::list<weak_ptr<Manager>> mManagers;
};

#endif // engine_h__
