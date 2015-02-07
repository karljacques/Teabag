//
//  osxEngine.m
//  YorkshireTea
//
//  Created by Karl Jacques on 26/05/2014.
//
//

#include "pch.h"
#include "engine.h"
#include "manager/abstract/spectatorManager.h"
#include <RakNetVersion.h>

Engine::Engine()
{
	SDL_Init( SDL_INIT_EVERYTHING );

    m_EngineShutdown = false;

    // Set the working directory
    char* basePath = SDL_GetBasePath();
#ifdef WIN32
	SetCurrentDirectoryA(basePath);
#else
	chdir(basePath);
#endif

	// Create Systems
    new EventSystem();
    mEntityManager = shared_ptr<EntityManager>( new EntityManager() );
	
	mPhysicsManager = shared_ptr<PhysicsManager>( new PhysicsManager() );
	mNetworkSystem = shared_ptr<NetworkSystem>( new NetworkSystem() );
	
	mRenderSystem = shared_ptr<RenderSystem>( new RenderSystem( mEntityManager.get() ) );
	mInputSystem = shared_ptr<InputSystem>( new InputSystem(  mRenderSystem->getSDLWindow() ) );

	// Create the camera manager and default camera
	mCameraManager = shared_ptr<CameraManager>( new CameraManager( mRenderSystem.get() ) );	
	mEntityManager->registerComponentManager<CameraComponent>(mCameraManager.get());
	EntID defaultCameraEntity = mEntityManager->createEntity();
	CameraComponent* defaultCamera = mEntityManager->createComponent<CameraComponent>( defaultCameraEntity );

	mCameraManager->createNewCamera( defaultCamera );

	mConsole = shared_ptr<OgreConsole>( new OgreConsole( mCameraManager.get() ) );

	mSpectatorManager = shared_ptr<SpectatorManager>( new SpectatorManager( mEntityManager.get() ) ); 

	mPlayerMgr = shared_ptr<PlayerManager>( new PlayerManager( mNetworkSystem.get() ));

	// Register managers to be updated
	registerManager(mEntityManager);
	registerManager(mPhysicsManager);
	registerManager(mNetworkSystem);
	registerManager(mRenderSystem);
	registerManager(mInputSystem);
	registerManager(mCameraManager);
	registerManager(mSpectatorManager);

	// Register Component Managers to receive components
	mEntityManager->registerComponentManager<PhysicsComponent>(mPhysicsManager.get());
	mEntityManager->registerComponentManager<RenderComponent>(mRenderSystem.get());
	mEntityManager->registerComponentManager<NetworkComponent>(mNetworkSystem.get());
	mEntityManager->registerComponentManager<SpectatorComponent>(mSpectatorManager.get());


	// Register the engine to receive input events
    this->setEventType(EV_CORE_KEY_PRESS||EV_CORE_KEY_RELEASE );

	// Register all the listening managers as listeners
	EventSystem* e = EventSystem::getSingletonPtr();
	e->registerListener(mPhysicsManager);
	e->registerListener(mRenderSystem);
	e->registerListener(mCameraManager);
	e->registerListener(mSpectatorManager);
	e->registerListener(mNetworkSystem);
	e->registerListener(mPlayerMgr);
	e->registerListener(mConsole);

	{
		// Create a spectator
		EntID ent = mEntityManager->createEntity();

		CameraComponent* comp = mEntityManager->createComponent<CameraComponent>(ent);
		mCameraManager->getCamera( comp );
		mEntityManager->getByID(ent)->addComponent(comp);

		SpectatorComponent* spec = mEntityManager->createComponent<SpectatorComponent>(ent);
		mEntityManager->getByID(ent)->addComponent(spec);

		PhysicsComponent* phys = mEntityManager->createComponent<PhysicsComponent>(ent);
		mPhysicsManager->initComponent(phys,new btSphereShape( 0.25f ) , 1, float3(0,2,0), Quat(0,0,0,1) );
		phys->body->setFriction(0);
		phys->body->setGravity(float3(0,0,0)); // Disable gravity on a spectator
		phys->body->setAngularFactor(float3(1.0f,1.0f,1.0f));
		mEntityManager->getByID(ent)->addComponent(phys);
		phys->body->setDamping( 0.95f, 1.0f );

		mCameraManager->lookAt( comp, float3( 0,0,0 ));
	}
	
	{
		// Create the ground
		EntID ground = mEntityManager->createEntity();

		RenderComponent* rend = mEntityManager->createComponent<RenderComponent>(ground);
		mRenderSystem->initComponent( rend );
		mRenderSystem->setAsBox(rend, float3(3.0f,0.4f,5.0f));
		mEntityManager->getByID(ground)->addComponent(rend);

		PhysicsComponent* phys = mEntityManager->createComponent<PhysicsComponent>(ground);
		mPhysicsManager->initComponent( phys,new btBoxShape( float3(1.5f, 0.2f,2.5f ) ), 0,float3(0,0,0), Quat(0,0,0,1));
		mEntityManager->getByID(ground)->addComponent(phys);
	}

	mSelf = std::shared_ptr<Engine>(this,  [=](Engine*){});
	EventSystem::getSingletonPtr()->registerListener(mSelf);

	mDebugDisplaySystem.reset( new DebugDisplaySystem( mCameraManager.get() ) );
	registerManager(mDebugDisplaySystem);
	mConsole->addCommand( "/net.connect", &Console_Net_Connect );
	mConsole->addCommand( "/player.setname", &Console_Set_Username);

	printm("RAKNET_PROTOCOL_VERSION_LOCAL:" + std::to_string( RAKNET_PROTOCOL_VERSION ));

}

Engine::~Engine()
{
	SDL_Quit();
}

void Engine::update()
{
	// Calculate timestep
	double dt = mTimeSinceLastUpdate.getMicrosecondsCPU();
	mTimeSinceLastUpdate.reset();

	EventSystem::getSingletonPtr()->update(dt);

	// Update systems and managers
    for( auto i=mManagers.begin(); i!=mManagers.end(); i++ )
	{
		shared_ptr<Manager> ptr = i->lock();
		if( ptr )
			ptr->update(dt);
	}

	if( mTimeSinceLastRender.getMicrosecondsCPU() > 16 )
	{
		// render after everything is updated
		mTimeSinceLastRender.reset();
		mRenderSystem->renderOneFrame();
	}

}

bool Engine::isShuttingDown()
{
    return m_EngineShutdown;
}

void Engine::handle( Event* e )
{
    switch( e->getEventType() )
    {
        case EV_CORE_KEY_PRESS:
			{
				KeyboardEvent* ke = e->getData<KeyboardEvent>();
				switch (ke->mKeycode)
				{
					case SDL_SCANCODE_ESCAPE: // Escape
					   m_EngineShutdown = true;
					break;

					case SDL_SCANCODE_X:
						{
							if( mNetworkSystem->isHost() )
							{
								float3 size( 0.3f,0.1f,0.3f );
								float mass = 0.5f;


								EntID box = mEntityManager->createEntity();

								RenderComponent* rend = mEntityManager->createComponent<RenderComponent>(box);
								mRenderSystem->initComponent( rend );
								mRenderSystem->setAsBox(rend, size);
								mEntityManager->getByID(box)->addComponent(rend);

								PhysicsComponent* phys = mEntityManager->createComponent<PhysicsComponent>(box);
								mPhysicsManager->initComponent( phys,new btBoxShape( size/2.0f ), mass,float3(0,3,0), Quat::RotateX( 3.14f ));
								mEntityManager->getByID(box)->addComponent(phys);

								NetworkComponent* net = mEntityManager->createComponent<NetworkComponent>(box);
								net->eGUID = mNetworkSystem->_find_free_guid();
								assert( net->eGUID != 0 );
								mEntityManager->getByID(box)->addComponent(net);

								Event* dyn = EventSystem::getSingletonPtr()->getEvent(EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX, 0, this);
								dyn->eGUID = net->eGUID;
								EventSystem::getSingletonPtr()->dispatchEvent( dyn );

								
							}
							
							

							break;
						}

					
				}
			}
        break;

		case EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX:
			{
				float3 size( 0.3f,0.1f,0.3f );
				float mass = 0.5f;

				EntID box = mEntityManager->createEntity();

				NetworkComponent* net = mEntityManager->createComponent<NetworkComponent>(box);
				net->eGUID = e->eGUID;

				RenderComponent* rend = mEntityManager->createComponent<RenderComponent>(box);
				mRenderSystem->initComponent( rend );
				mRenderSystem->setAsBox(rend, size);
				mEntityManager->getByID(box)->addComponent(rend);

				PhysicsComponent* phys = mEntityManager->createComponent<PhysicsComponent>(box);
				mPhysicsManager->initComponent( phys,new btBoxShape( size/2.0f ), mass,float3(0,3,0), Quat::RotateX( 3.14f ));
				mEntityManager->getByID(box)->addComponent(phys);
			}
			break;

    }
}


void Engine::SetAsClient(const char* ip)
{
	mNetworkSystem->setAsClient();
	mNetworkSystem->connect(ip);
}

void Engine::registerManager(weak_ptr<Manager> mgr)
{
	mManagers.push_back(mgr);
}

void Engine::removeManager(weak_ptr<Manager> mgr)
{
	mManagers.remove_if([mgr](weak_ptr<Manager> p){
			shared_ptr<Manager> p1 = p.lock();
			shared_ptr<Manager> p2 = mgr.lock();

			if( p1 && p2 )
				return p1 == p2;
			return false;
	});
}

