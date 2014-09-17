//
//  osxEngine.m
//  YorkshireTea
//
//  Created by Karl Jacques on 26/05/2014.
//
//

#include "pch.h"
#include <SDL.h>

#include "engine.h"
#include "engine/core/component/render/cameraComponent.h"
#include "engine/core/component/control/spectatorControlComponent.h"
#include "engine/world/entityManager.h"

#include "engine/core/graphics/UI/OgreConsoleForGorilla.h"

void createBlock( Engine* eng, float3 size, float3 pos, float yaw )
{
	Entity* cube = eng->createEntity();
	PositionComponent* n = new PositionComponent();
	cube->addComponent( n );

	RenderComponent* c =  new RenderComponent( eng->getRenderSystem(), n );
	c->setAsBox( size.x,size.y,size.z);
	n->registerListener( c );
	cube->addComponent(c);

	PhysicsComponent* p = new PhysicsComponent( eng->getPhysicsManager(),n );
	btCollisionShape* shapex = new btBoxShape( size/2 );
	p->initialise( shapex, 0.2f, pos );	
	p->registerListener( c );
	p->registerListener( n );
	n->registerListener( p );
	n->setOrientation(Quat::RotateY(yaw/57.6f));
	n->setPosition(pos);
	cube->addComponent( p );
}



Engine::Engine()
{
	SDL_Init( SDL_INIT_EVERYTHING );

    m_EngineShutdown = false;

	// Create Systems
    mRenderSystem = new RenderSystem();
    mEventSystem = new EventSystem();
    mInputSystem = new InputSystem( mEventSystem, mRenderSystem->getSDLWindow() );
	mPhysicsManager = new PhysicsManager();
	mNetworkSystem = new ServerNetworkSystem();
	
	// Register the engine to receive input events
    this->setEventType(EV_CORE_KEY_PRESS||EV_CORE_KEY_RELEASE );
    mEventSystem->registerListener( this );

	

	////////////////////////////
	// SANDBOX BEYOND THIS POINT
	////////////////////////////
	
	// Create a cube
	{
		Entity* cube = createEntity();
		PositionComponent* n = new PositionComponent();
		cube->addComponent( n );

		RenderComponent* c =  new RenderComponent( mRenderSystem, n );
		n->registerListener( c );
		c->setAsBox(1.0f,3.0f,1.0f);
		cube->addComponent(c);

		PhysicsComponent* p = new PhysicsComponent( mPhysicsManager,n );
		btCollisionShape* shapex = new btBoxShape( float3(1.0f,3.0f, 1.0f )/2 );
		p->initialise( shapex, 0.1, float3(0,51.0,0), Quat( 1.0, 0.9,0,0.7 ) );	
		p->registerListener( c );
		p->registerListener( n );
		cube->addComponent( p );
	}
	
	// Static Geometry
	EntityManager* entityManager = new EntityManager();
	mStaticGeometry = new StaticGeometry( entityManager, mRenderSystem, mPhysicsManager );

	mStaticGeometry->addGeometry( float3( 0,0.0f,0.0f), float3( 100.0f, 1.0f, 100.0f ), float3(0,0,0));
	mStaticGeometry->addGeometry( float3( 0,20.0f, 0.0f), float3( 10.0f,1.0f, 10.0f), float3( 0,0,45.0/57.0 ));
	mStaticGeometry->addGeometry( float3( -10.0f,10.0f, 0.0f), float3( 10.0f,1.0f, 10.0f), float3( 0,0,-45.0/57.0 ));

	// sides
	mStaticGeometry->addGeometry( float3( 50,0.0f,0.0f), float3( 1.0f, 10.0f, 100.0f ));
	mStaticGeometry->addGeometry( float3( -50,0.0f,0.0f), float3( 1.0f, 10.0f, 100.0f ));

	mStaticGeometry->addGeometry( float3( 0,0.0f,50.0f), float3( 100.0f, 10.0f, 1.0f ));
	mStaticGeometry->addGeometry( float3( 0,0.0f,-50.0f), float3( 100.0f, 10.0f, 1.0f ));
	// Camera
	Entity* camera = createEntity();

	PositionComponent* cpc = new PositionComponent();
	CameraComponent* cc = new CameraComponent( mRenderSystem, cpc );
	PhysicsComponent* cphyc = new PhysicsComponent( mPhysicsManager, cpc );
	SpectatorControlComponent* s = new SpectatorControlComponent( cphyc );

	mEventSystem->registerListener( s );

	camera->addComponent(cc);
	camera->addComponent(cpc);
	camera->addComponent(cphyc);
	camera->addComponent(s);

	cpc->registerListener( cphyc );
	s->registerListener(cphyc);

	cpc->setPosition( float3(0,40,20 ));
	//cphyc->registerListener( cc );

	// Create tower
	for( float x = 20; x<26; x+=2.0f )
	{
		for( float y = 0.75f; y<16.0f; y+=2.0f )
		{
			createBlock( this, float3( 2,1,6), float3(x,y,20), 0);
		}
		
	}

	for( float z = 18.0f; z < 23.5f; z+=2.0f )
		for( float y = 1.5f;y<16.0f; y+=2.0f )
		{
			createBlock( this, float3( 2,1,6), float3(22.0f,y,z), 90);
		}


		// Create console
		Gorilla::Silverback* silverback = new Gorilla::Silverback();
		silverback->loadAtlas("dejavu");
		Gorilla::Screen* UIScreen = silverback->createScreen( mRenderSystem->getViewport(),"dejavu" );

		OgreConsole* console = new OgreConsole();

		console->init(UIScreen );

		mEventSystem->registerListener(console);
		
}


Engine::~Engine()
{
	SDL_Quit();

    delete mEventSystem;
    delete mInputSystem;
	delete mRenderSystem;
}

void Engine::update()
{
    // Make sure to pump messages in all render windows
    Ogre::WindowEventUtilities::messagePump();

	// Calculate timestep
	double dt = mTimeSinceLastUpdate.getMilliseconds()/1000.0;
	mTimeSinceLastUpdate.reset();

	// Update systems and managers
    mInputSystem->update();
    mEventSystem->handleEvents();
	mPhysicsManager->update( dt );
	mNetworkSystem->receive();

	// Update game entities
	for( auto i = mEntities.begin(); i != mEntities.end(); i++ )
		(*i)->update( dt );

    // render after everything is updated
    mRenderSystem->renderOneFrame();
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
            KeyboardEvent* ke = static_cast<KeyboardEvent*>(e);
            switch (ke->mKeycode)
            {
				case SDL_SCANCODE_ESCAPE: // Escape
                   m_EngineShutdown = true;
                break;

				case SDL_SCANCODE_C:
					if( ke->mPressed )
						spawnNewCube();
					break;

            }
		
        break;
    }
}

Entity* Engine::createEntity()
{
	Entity* ent = new Entity();
	mEntities.push_back(ent);
	return ent;
}

void Engine::spawnNewCube()
{
	{
		Entity* cube = createEntity();
		PositionComponent* n = new PositionComponent();
		cube->addComponent( n );
		
		float x = (rand() % 5 )+1;
		float y = (rand() % 5 )+1;
		float z = (rand() % 5 )+1;
		RenderComponent* c =  new RenderComponent( mRenderSystem, n );
		n->registerListener( c );
		c->setAsBox(x,y,z);
		cube->addComponent(c);

		PhysicsComponent* p = new PhysicsComponent( mPhysicsManager,n );
		btCollisionShape* shapex = new btBoxShape( float3(x/2,y/2,z/2) );
		p->initialise( shapex, (rand() % 20 )+1, float3(0,51.0,0), Quat( 1.0, 0.9,0,0.7 ) );	
		p->registerListener( c );
		p->registerListener( n );
		cube->addComponent( p );
	}
}

