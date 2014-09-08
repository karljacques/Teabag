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


Engine::Engine()
{
	SDL_Init( SDL_INIT_EVERYTHING );
    m_EngineShutdown = false;

	// Create Systems
    mRenderSystem = new RenderSystem();
    mEventSystem = new EventSystem();
    mInputSystem = new InputSystem( mEventSystem, mRenderSystem->getSDLWindow() );
	mPhysicsManager = new PhysicsManager();

	// Register the engine to receive input events
    this->setEventType(EV_CORE_KEY_PRESS||EV_CORE_KEY_RELEASE );
    mEventSystem->registerListener( this );
	
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
		p->initialise( shapex, 5.0, float3(0,51.0,0), Quat( 1.0, 0.9,0,0.7 ) );	
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

Camera* Engine::createCamera()
{
    Camera* cam = new Camera( mRenderSystem->getRenderWindow(), mRenderSystem->getSceneMgr(), mRenderSystem->getRootSceneNode() );
    mRenderSystem->getCameraList()->push_back( cam );

    cam->init();
    return cam;
}

Terrain* Engine::createTerrain()
{
    return new Terrain( mRenderSystem->getSceneMgr() );
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