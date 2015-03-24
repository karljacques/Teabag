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
	entitysys::registerComponentManager<PlayerComponent>(mPlayerMgr.get());

	// Register all the listening managers as listeners
	eventsys::registerListener(mPhysicsManager);
	eventsys::registerListener(mRenderManager);
	eventsys::registerListener(mCameraManager);
	eventsys::registerListener(mSpectatorManager);
	eventsys::registerListener(mNetworkManager);
	eventsys::registerListener(mPlayerMgr);
	eventsys::registerListener(mConsole);
	eventsys::registerListener(mSnapshotManager);

	mSelf = std::shared_ptr<Engine>(this,  [=](Engine*){});
	eventsys::registerListener(mSelf);

	mDebugDisplaySystem.reset( new DebugDisplaySystem( mCameraManager.get() ) );
	registerManager(mDebugDisplaySystem);
	mConsole->addCommand( "/net.connect", &Console_Net_Connect );
	mConsole->addCommand( "/player.setname", &Console_Set_Username);

	printm("RAKNET_PROTOCOL_VERSION_LOCAL:" + std::to_string( RAKNET_PROTOCOL_VERSION ));

	eventsys::update();

	// Default frame
	std::shared_ptr<Frame> defaultFrame = std::shared_ptr<Frame>( new GameModeFree( ) );
	queueFrame(defaultFrame);
	
	// Create the ground
	{
		// Create the ground
		EntID ground = entitysys::createEntity();

		TransformComponent* trans = entitysys::createComponent<TransformComponent>(ground);
		entitysys::getByID(ground)->addComponent(trans);

		RenderComponent* rend = entitysys::createComponent<RenderComponent>(ground);
		mRenderManager->initComponent( rend );
		mRenderManager->setAsBox(rend, float3(20.0f,0.4f,20.0f), "Grid/Grey" );
		entitysys::getByID(ground)->addComponent(rend);


		PhysicsComponent* phys = entitysys::createComponent<PhysicsComponent>(ground);
		mPhysicsManager->initComponent( phys,new btBoxShape( float3(10.0f, 0.2f,10.0f ) ), 0);
		entitysys::getByID(ground)->addComponent(phys);

	}

	// Create the ceiling
	{
		EntID ground = entitysys::createEntity();

		TransformComponent* trans = entitysys::createComponent<TransformComponent>(ground);
		entitysys::getByID(ground)->addComponent(trans);

		RenderComponent* rend = entitysys::createComponent<RenderComponent>(ground);
		mRenderManager->initComponent( rend );
		mRenderManager->setAsBox(rend, float3(20.0f,0.4f,20.0f), "Grid/Grey" );
		entitysys::getByID(ground)->addComponent(rend);

		PhysicsComponent* phys = entitysys::createComponent<PhysicsComponent>(ground);
		mPhysicsManager->initComponent( phys,new btBoxShape( float3(10.0f, 0.2f,10.0f ) ), 0);
		entitysys::getByID(ground)->addComponent(phys);

		ENT_SET_TRANSFORM( ground, float3(0,4.0f,0 ), Quat(0,0,0,1));
	}

	EntID wall = entitysys::createEntity();

	TransformComponent* trans = entitysys::createComponent<TransformComponent>(wall);
	entitysys::getByID(wall)->addComponent(trans);

	RenderComponent* rend = entitysys::createComponent<RenderComponent>(wall);
	mRenderManager->initComponent( rend );
	mRenderManager->setAsBox(rend, float3(20.0f,4.0f,0.4f), "Grid/Orange" );
	entitysys::getByID(wall)->addComponent(rend);

	PhysicsComponent* phys = entitysys::createComponent<PhysicsComponent>(wall);
	mPhysicsManager->initComponent( phys,new btBoxShape( float3(10.0f, 2.0f,0.2f ) ), 0);
	entitysys::getByID(wall)->addComponent(phys);

	ENT_SET_TRANSFORM( wall, float3(0.0f,2.0f,10.0f), Quat(0,0,0,1));
	EntityPrototype* wallProto = new EntityPrototype( entitysys::getByID(wall) );

	Entity* wall2 = wallProto->spawn();
	ENT_SET_TRANSFORM( wall2->ID, float3( 0.0f, 2.0f, -10.0f), Quat(0,0,0,1));

	Entity* wall3 = wallProto->spawn();
	ENT_SET_TRANSFORM( wall3->ID, float3( 10.0f, 2.0f, 0.0f), Quat::RotateY( 3.14/2.0f ));

	Entity* wall4 = wallProto->spawn();
	ENT_SET_TRANSFORM( wall4->ID, float3( -10.0f, 2.0f, 0.0f), Quat::RotateY( 3.14/2.0f ));

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

	// Update current frame
	FrameState state = mFrames.back()->update(); // Must have at least one frame or the engine will never do anything.

	// See if frame is ending
	if( state == FR_BEGIN_TERMINATION )
	{
		rewindFrame(mFrames.back());
	}

	// Background update the other frames
	if( mFrames.size() > 1 )
	{
		for( auto i=mFrames.begin(); i != ( mFrames.end() - 1 ); i++ )
		{
			(*i)->backgroundUpdate();
		}
	}

	// Check there is at least one frame - if there isn't terminate the engine
	if( mFrames.size() == 0 )
		m_EngineShutdown = true;

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

void Engine::queueFrame( std::shared_ptr<Frame> frame)
{
	// Set state of old frame to background
	if( mFrames.size() > 0 )
		mFrames.back()->mFrameState = FR_BACKGROUND;

	mFrames.push_back(frame);
	frame->mFrameState = FR_ACTIVE;

	eventsys::registerListener( frame );
}

void Engine::rewindFrame( std::shared_ptr<Frame> frame)
{
	frame->mFrameState = FR_TERMINATED;
	eventsys::deregisterListener(frame);
	mFrames.pop_back();

	// Set current frame to active
	if( mFrames.size() > 0 )
		mFrames.back()->mFrameState = FR_ACTIVE;
}

