#ifndef engine_h__
#define engine_h__
/*
//  Teabag
//  Created as 'Yorkshire Tea' in XCode on OSX by Karl Jacques on 26/05/2014.
*/

#include "engine/component/render/renderManager.h"
#include "engine/component/physics/physicsManager.h"

#include "engine/component/render/cameraManager.h"
#include "engine/component/spectator/spectatorManager.h"
#include "engine/component/player/playerManager.h"
#include "engine/component/network/networkManager.h"

#include "engine/manager/debug/debugDisplaySystem.h"
#include "engine/manager/debug/ogreConsole.h"

#include "engine/core/networkSystem.h"
#include "engine/core/inputSystem.h"
#include "engine/core/physicsSystem.h"
#include "engine/core/renderSystem.h"
#include "engine/core/entitySystem.h"

#include "engine/factory/entityPrototype.h"

class OgreConsole;

class Engine : public EventListener
{
public:
        
    Engine();
	~Engine();
        
    void update();
    void handle( Event* e );
        
    bool isShuttingDown();

	// Manager methods
	void registerManager( std::weak_ptr<Manager> mgr );
	void removeManager( std::weak_ptr<Manager> mgr );

	// Managers getters
	RenderManager*		getRenderSystem(){ return mRenderSystem.get(); };
	PhysicsManager*     getPhysicsManager(){ return mPhysicsManager.get(); };
	NetworkManager*		getNetworkSystem(){return mNetworkSystem.get(); };
	CameraManager*		getCameraManager(){return mCameraManager.get(); };
	PlayerManager*		getPlayerManager(){ return mPlayerMgr.get(); };

	// System
	Ogre::Timer mTimeSinceLastUpdate;
	Ogre::Timer mTimeSinceLastRender;

	// Console functions/Engine manipulation
	void SetAsClient( const char* ip );

private:
	
	bool m_EngineShutdown;
	
	std::shared_ptr<DebugDisplaySystem> mDebugDisplaySystem;
	std::shared_ptr<Engine> mSelf;
	std::shared_ptr<PlayerManager> mPlayerMgr;

	std::shared_ptr<PhysicsManager> mPhysicsManager;
	std::shared_ptr<CameraManager> mCameraManager;
	std::shared_ptr<OgreConsole> mConsole;
	std::shared_ptr<SpectatorManager> mSpectatorManager;

	std::shared_ptr<RenderManager> mRenderSystem;
	std::shared_ptr<NetworkManager> mNetworkSystem;

	std::list<std::weak_ptr<Manager>> mManagers;

	std::unordered_map<std::string, EntityPrototype*> mPrototypes;
};

#endif // engine_h__
