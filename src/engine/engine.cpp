//
//  osxEngine.m
//  YorkshireTea
//
//  Created by Karl Jacques on 26/05/2014.
//
//

#include "pch.h"
#include "engine.h"
#include "manager\network\clientNetworkSystem.h"
#include "manager\network\serverNetworkSystem.h"
#include "manager\abstract\spectatorManager.h"

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
    mEntityManager = new EntityManager();

	mPhysicsManager = new PhysicsManager();
	mNetworkSystem = new ServerNetworkSystem( );
	
	mRenderSystem = new RenderSystem( mEntityManager );
	mInputSystem = new InputSystem(  mRenderSystem->getSDLWindow() );
	mCameraManager = new CameraManager( mRenderSystem );

	mSpectatorManager = new SpectatorManager( mEntityManager );

	// Register the engine to receive input events
    this->setEventType(EV_CORE_KEY_PRESS||EV_CORE_KEY_RELEASE );
	EventSystem::getSingletonPtr()->registerListener( this );

	// Register all the listening managers as listeners
	EventSystem* e = EventSystem::getSingletonPtr();
	e->registerListener(mPhysicsManager);
	e->registerListener(mRenderSystem);
	e->registerListener(mCameraManager);
	e->registerListener(mSpectatorManager);

	{
		// Create a spectator
		Entity* ent = mEntityManager->createEntity();

		CameraComponent* comp = mCameraManager->createComponent(ent->LUID);
		mCameraManager->createNewCamera( comp );
		ent->addComponent(comp);

		SpectatorComponent* spec = mSpectatorManager->createComponent(ent->LUID);
		ent->addComponent(spec);

		PhysicsComponent* phys = mPhysicsManager->createComponent(ent->LUID);
		mPhysicsManager->initComponent(phys,new btSphereShape( 1.0f ) , 1, float3(40,20,0), Quat(0,0,0,1) );
		phys->body->setGravity(float3(0,0,0)); // Disable gravity on a spectator
		phys->body->setAngularFactor(float3(1.0f,1.0f,1.0f));
		ent->addComponent(phys);
		phys->body->setDamping( 0.9f, 1.0f );

		mCameraManager->lookAt( comp, float3( 0,0,0 ));
	}
	
	{
		// Create a Tumbler
		mTumbler[0] = mEntityManager->createEntity();

		RenderComponent* rend = mRenderSystem->createComponent(mTumbler[0]->LUID);
		mRenderSystem->initComponent( rend );
		mRenderSystem->setAsBox(rend, float3(100,2,100));
		mTumbler[0]->addComponent(rend);

		PhysicsComponent* phys = mPhysicsManager->createComponent(mTumbler[0]->LUID);
		mPhysicsManager->initComponent( phys,new btBoxShape( float3(50.0f, 1.0f,50.0f ) ), 0,float3(0,0,0), Quat(0,0,0,1));
		mTumbler[0]->addComponent(phys);
	}
	{

		// Create a static ground
		mTumbler[1] = mEntityManager->createEntity();

		RenderComponent* rend = mRenderSystem->createComponent(mTumbler[1]->LUID);
		mRenderSystem->initComponent( rend );
		mRenderSystem->setAsBox(rend, float3(100,2,100));
		mTumbler[1]->addComponent(rend);

		PhysicsComponent* phys = mPhysicsManager->createComponent(mTumbler[1]->LUID);
		mPhysicsManager->initComponent( phys,new btBoxShape( float3(50.0f, 1.0f,50.0f ) ), 0,float3(50,50,0), Quat::RotateZ(3.14/2.0f));
		mTumbler[1]->addComponent(phys);


	}
	{

		// Create a static ground
		mTumbler[2] = mEntityManager->createEntity();

		RenderComponent* rend = mRenderSystem->createComponent(mTumbler[2]->LUID);
		mRenderSystem->initComponent( rend );
		mRenderSystem->setAsBox(rend, float3(100,2,100));
		mTumbler[2]->addComponent(rend);

		PhysicsComponent* phys = mPhysicsManager->createComponent(mTumbler[2]->LUID);
		mPhysicsManager->initComponent( phys,new btBoxShape( float3(50.0f, 1.0f,50.0f ) ), 0,float3(0,100,0), Quat::RotateZ(3.14f));
		mTumbler[2]->addComponent(phys);


	}
	{

		// Create a static ground
		mTumbler[3] = mEntityManager->createEntity();

		RenderComponent* rend = mRenderSystem->createComponent(mTumbler[3]->LUID);
		mRenderSystem->initComponent( rend );
		mRenderSystem->setAsBox(rend, float3(100,2,100));
		mTumbler[3]->addComponent(rend);

		PhysicsComponent* phys = mPhysicsManager->createComponent(mTumbler[3]->LUID);
		mPhysicsManager->initComponent( phys,new btBoxShape( float3(50.0f, 1.0f,50.0f ) ), 0,float3(-50,50,0), Quat::RotateZ(-3.14/2.0f));
		mTumbler[3]->addComponent(phys);


	}

	{

		// Create a static ground
		mTumbler[4] = mEntityManager->createEntity();

		RenderComponent* rend = mRenderSystem->createComponent(mTumbler[4]->LUID);
		mRenderSystem->initComponent( rend );
		mRenderSystem->setAsBox(rend, float3(100,2,100));
		mTumbler[4]->addComponent(rend);

		PhysicsComponent* phys = mPhysicsManager->createComponent(mTumbler[4]->LUID);
		mPhysicsManager->initComponent( phys,new btBoxShape( float3(50.0f, 1.0f,50.0f ) ), 0,float3(0,50,50), Quat::RotateX(-3.14/2.0f));
		mTumbler[4]->addComponent(phys);


	}



	/* Create console - Singleton
		Registers itself as an event listener */
	new OgreConsole(this);

	OgreConsole::getSingleton().addCommand( "net.connect", &Console_Net_Connect );
	OgreConsole::getSingleton().addCommand( "net.status", &Console_Net_Status );

	OgreConsole::getSingleton().addCommand( "geo.spawn", &Console_Geometry_Spawn );
	
	mTumblerSpeed = 0;
}

Engine::~Engine()
{
	SDL_Quit();

    delete mInputSystem;
	delete mRenderSystem;
}

void Engine::update()
{
    // Make sure to pump messages in all render windows
    Ogre::WindowEventUtilities::messagePump();

	// Calculate timestep
	double dt = mTimeSinceLastUpdate.getMicrosecondsCPU();
	mTimeSinceLastUpdate.reset();
	OgreConsole::getSingletonPtr()->print( "Loop Time: " + std::to_string( dt ) );
	// Update systems and managers
    mInputSystem->update();

	static Ogre::Timer profile;
	profile.reset();
	EventSystem::getSingletonPtr()->handleEvents();
	OgreConsole::getSingletonPtr()->print( "Event Time: " + std::to_string(profile.getMicroseconds()/1000.0f) );
	mPhysicsManager->update( dt );
	mSpectatorManager->update();

	mNetworkSystem->receive();

    // render after everything is updated
    mRenderSystem->renderOneFrame();

	// Update tumbler
	for( int i=0; i<4; i++ )
	{
		Entity* tumbler = mTumbler[i];

		// Get Rotation
		Quat rot = tumbler->getComponent<PhysicsComponent>()->orientation;

		// Get position
		float3 pos = tumbler->getComponent<PhysicsComponent>()->position;

		// Increase rotation
		rot= rot*Quat::RotateZ(mTumblerSpeed*dt*0.000001f);

		// Calculate new position
		pos = float3( 0,50,0 ) + rot*(float3(0,50,0));

		// Send out event
		Event* e = EventSystem::getSingletonPtr()->getEvent( EV_CORE_TRANSFORM_UPDATE, tumbler->LUID );
		TransformEvent* te = e->createEventData<TransformEvent>();
		te->mFloat3_1 = pos;
		te->mQuaternion = rot;
		EventSystem::getSingletonPtr()->dispatchEvent(e);


	}

	int back = mTumbler[4]->LUID;
	// Send out event
	Event* e = EventSystem::getSingletonPtr()->getEvent( EV_CORE_TRANSFORM_UPDATE, back );
	TransformEvent* te = e->createEventData<TransformEvent>();
	te->mFloat3_1 = float3(0,50,50);
	te->mQuaternion = Quat::RotateZ( mTumbler[0]->getComponent<PhysicsComponent>()->orientation.ToEulerXYZ().z )*Quat::RotateX(-3.14/2.0f);
	EventSystem::getSingletonPtr()->dispatchEvent(e);
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

							float3 size( 
								((rand()% 50) /10.0f ) +1,
								((rand()% 50) /10.0f ) +1,
								((rand()% 50) /10.0f  )+1);

							float mass = ((rand()%50) / 10.0f) + 1;


							Entity* box = mEntityManager->createEntity();

							RenderComponent* rend = mRenderSystem->createComponent(box->LUID);
							mRenderSystem->initComponent( rend );
							mRenderSystem->setAsBox(rend, size);
							box->addComponent(rend);

							PhysicsComponent* phys = mPhysicsManager->createComponent(box->LUID);
							mPhysicsManager->initComponent( phys,new btBoxShape( size/2.0f ), mass,float3(0,50,0), Quat::RotateX( 3.14f ));
							box->addComponent(phys);
							break;
						}
					case SDL_SCANCODE_LEFTBRACKET:
						mTumblerSpeed-=0.25f;
						break;
					case SDL_SCANCODE_RIGHTBRACKET:
						mTumblerSpeed+=0.25f;
						break;
				}
			}
        break;

    }
}


void Engine::SetAsClient(const char* ip)
{
	EventSystem::getSingletonPtr()->deregisterListener(mNetworkSystem);
	delete mNetworkSystem;

	this->mNetworkSystem = new ClientNetworkSystem( );
	EventSystem::getSingletonPtr()->registerListener(this->mNetworkSystem);
	static_cast<ClientNetworkSystem*>(mNetworkSystem)->connect( ip );

	
}
