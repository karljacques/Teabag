//
//  osxEngine.m
//  YorkshireTea
//
//  Created by Karl Jacques on 26/05/2014.
//
//

#include "pch.h"
#include "engine.h"

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
    eventsys::init();
	network::init();
	physics::init();
	render::init();
	
	mPhysicsManager = std::shared_ptr<PhysicsManager>( new PhysicsManager() );
	mNetworkManager = network::getNetworkManager();
	mRenderManager = std::shared_ptr<RenderManager>( new RenderManager( ) );

	input::init(render::getSDLWindow());

	// Create the camera manager and default camera
	mCameraManager = std::shared_ptr<CameraManager>( new CameraManager() );	
	entitysys::registerComponentManager<CameraComponent>(mCameraManager.get());
	EntID defaultCameraEntity = entitysys::createEntity();
	CameraComponent* defaultCamera = entitysys::createComponent<CameraComponent>( defaultCameraEntity );

	mCameraManager->createNewCamera( defaultCamera );
	mConsole = std::shared_ptr<OgreConsole>( new OgreConsole( mCameraManager.get() ) );

	mSpectatorManager = std::shared_ptr<SpectatorManager>( new SpectatorManager( ) ); 
	mPlayerMgr = std::shared_ptr<PlayerManager>( new PlayerManager( ));
	mSnapshotManager = std::shared_ptr<SnapshotManager>( new SnapshotManager( mNetworkManager.get() ) );

	// Register managers to be updated
	registerManager(mPhysicsManager);
	registerManager(mNetworkManager);
	registerManager(mRenderManager);
	registerManager(mCameraManager);
	registerManager(mSpectatorManager);
	registerManager(mNetworkManager);
	registerManager(mSnapshotManager);

	// Register Component Managers to receive components
	entitysys::registerComponentManager<PhysicsComponent>(mPhysicsManager.get());
	entitysys::registerComponentManager<RenderComponent>(mRenderManager.get());
	entitysys::registerComponentManager<NetworkComponent>(mNetworkManager.get());
	entitysys::registerComponentManager<SpectatorComponent>(mSpectatorManager.get());


	// Register the engine to receive input events
    this->setEventType(EV_CORE_KEY_PRESS||EV_CORE_KEY_RELEASE );

	// Register all the listening managers as listeners
	eventsys::registerListener(mPhysicsManager);
	eventsys::registerListener(mRenderManager);
	eventsys::registerListener(mCameraManager);
	eventsys::registerListener(mSpectatorManager);
	eventsys::registerListener(mNetworkManager);
	eventsys::registerListener(mPlayerMgr);
	eventsys::registerListener(mConsole);
	eventsys::registerListener(mSnapshotManager);

	{
		// Create a spectator
		EntID ent = entitysys::createEntity();

		TransformComponent* trans = entitysys::createComponent<TransformComponent>(ent);
		entitysys::getByID(ent)->addComponent(trans);

		MotionComponent* motion = entitysys::createComponent<MotionComponent>(ent);
		entitysys::getByID(ent)->addComponent(motion);

		SpectatorComponent* spec = entitysys::createComponent<SpectatorComponent>(ent);
		entitysys::getByID(ent)->addComponent(spec);

		PhysicsComponent* phys = entitysys::createComponent<PhysicsComponent>(ent);
		mPhysicsManager->initComponent(phys,new btSphereShape( 0.25f ) , 1);
		phys->body->setFriction(0);
		phys->body->setGravity(float3(0,0,0)); // Disable gravity on a spectator
		phys->body->setAngularFactor(float3(1.0f,1.0f,1.0f));
		entitysys::getByID(ent)->addComponent(phys);
		phys->body->setDamping( 0.95f, 1.0f );

		EntityPrototype* spectatorPrototype = new EntityPrototype( entitysys::getByID(ent) );
		entitysys::destroyEntity(entitysys::getByID(ent));

		Entity* spectator = spectatorPrototype->spawn();

		CameraComponent* comp = entitysys::createComponent<CameraComponent>(spectator->ID);
		mCameraManager->getCamera( comp );
		spectator->addComponent(comp);

		mCameraManager->lookAt( comp, float3( 0,0,0 ));
	}
	
	{
		// Create the ground
		EntID ground = entitysys::createEntity();

		TransformComponent* trans = entitysys::createComponent<TransformComponent>(ground);
		entitysys::getByID(ground)->addComponent(trans);

		RenderComponent* rend = entitysys::createComponent<RenderComponent>(ground);
		mRenderManager->initComponent( rend );
		mRenderManager->setAsBox(rend, float3(20.0f,0.4f,20.0f), "SimpleGround/Textured" );
		entitysys::getByID(ground)->addComponent(rend);

		PhysicsComponent* phys = entitysys::createComponent<PhysicsComponent>(ground);
		mPhysicsManager->initComponent( phys,new btBoxShape( float3(10.0f, 0.2f,10.0f ) ), 0);
		entitysys::getByID(ground)->addComponent(phys);
	}

	mSelf = std::shared_ptr<Engine>(this,  [=](Engine*){});
	eventsys::registerListener(mSelf);

	mDebugDisplaySystem.reset( new DebugDisplaySystem( mCameraManager.get() ) );
	registerManager(mDebugDisplaySystem);
	mConsole->addCommand( "/net.connect", &Console_Net_Connect );
	mConsole->addCommand( "/player.setname", &Console_Set_Username);

	printm("RAKNET_PROTOCOL_VERSION_LOCAL:" + std::to_string( RAKNET_PROTOCOL_VERSION ));

	// Create box prototype
	float3 size( 1.0f,1.0f,1.0f );
	float mass = 4.0f;

	EntID box = entitysys::createEntity();

	TransformComponent* trans = entitysys::createComponent<TransformComponent>(box);
	entitysys::getByID(box)->addComponent(trans);

	MotionComponent* motion = entitysys::createComponent<MotionComponent>(box);
	entitysys::getByID(box)->addComponent(motion);

	RenderComponent* rend = entitysys::createComponent<RenderComponent>(box);
	mRenderManager->initComponent( rend );
	mRenderManager->setAsBox(rend, size);
	entitysys::getByID(box)->addComponent(rend);

	PhysicsComponent* phys = entitysys::createComponent<PhysicsComponent>(box);
	mPhysicsManager->initComponent( phys,new btBoxShape( size/2.0f ), mass );
	entitysys::getByID(box)->addComponent(phys);

	mPrototypes["box"] = new EntityPrototype( entitysys::getByID(box) );
	entitysys::destroyEntity( entitysys::getByID(box) );
}

Engine::~Engine()
{
	SDL_Quit();
}

void Engine::update()
{
	// Calculate timestep
	double dt = mTimeSinceLastUpdate.getMicroseconds();
	mTimeSinceLastUpdate.reset();

	eventsys::update();
	network::update();
	input::update();
	physics::update( dt );

	// Update systems and managers
    for( auto i=mManagers.begin(); i!=mManagers.end(); i++ )
	{
		std::shared_ptr<Manager> ptr = i->lock();
		if( ptr )
			ptr->update(dt);
	}
		// render after everything is updated
		mTimeSinceLastRender.reset();
		render::update();

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
							if( network::getMode() == 1 )
							{
								Entity* box = mPrototypes["box"]->spawn();

								assert( box->hasComponent<TransformComponent>() );
								ENT_SET_TRANSFORM( box->ID, float3(0,20,0), Quat(0,0,0,1) );

								NetworkComponent* net = entitysys::createComponent<NetworkComponent>(box->ID);
								net->eGUID = mNetworkManager->_find_free_guid();
								assert( net->eGUID != 0 );
								box->addComponent(net);

								Event* dyn = eventsys::get(EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX, 0, this);
								dyn->eGUID = net->eGUID;
								eventsys::dispatch( dyn );		

								printm( "Created box with GUID: " + std::to_string(net->eGUID ) + " and ID:" + std::to_string(box->ID));
							}
							break;
						}
				}
			}
        break;

		case EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX:
			{
				Entity* box = mPrototypes["box"]->spawn();
				NetworkComponent* net = entitysys::createComponent<NetworkComponent>(box->ID);
				net->eGUID = e->eGUID;
				printm( "Created box with GUID: " + std::to_string(net->eGUID ) + " and ID:" + std::to_string(box->ID));
				box->addComponent( net );
			}
			break;

    }
}

void Engine::registerManager(std::weak_ptr<Manager> mgr)
{
	mManagers.push_back(mgr);
}

void Engine::removeManager(std::weak_ptr<Manager> mgr)
{
	mManagers.remove_if([mgr](std::weak_ptr<Manager> p){
			std::shared_ptr<Manager> p1 = p.lock();
			std::shared_ptr<Manager> p2 = mgr.lock();

			if( p1 && p2 )
				return p1 == p2;
			return false;
	});
}

