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
#include "engine/world/entityManager.h"
#include "engine/world/staticGeometry.h"

Engine::Engine()
{
	SDL_Init( SDL_INIT_EVERYTHING );
    m_EngineShutdown = false;

	// Create Systems
    mRenderSystem = new RenderSystem();
    mEventSystem = new EventSystem();
    mInputSystem = new InputSystem( mEventSystem );
	mPhysicsManager = new PhysicsManager();

	// Register the engine to receive input events
    this->setEventType(EV_KeyPress||EV_KeyRelease );
    mEventSystem->registerListener( this );

	// Create a cube, spin it
	Entity* cube = createEntity();
	RenderComponent* c =  new RenderComponent( mRenderSystem );
	c->setAsBox(1.0f,3.0f,1.0f);
	cube->addComponent(c);

	PositionComponent* n = new PositionComponent();
	cube->addComponent( n );
	n->registerListener( c );
	
	PhysicsComponent* p = new PhysicsComponent( mPhysicsManager );
	btCollisionShape* shapex = new btBoxShape( float3(1.0f,3.0f, 1.0f )/2 );
	p->initialise( shapex, 5.0, float3(0,51.0,0), Quat( 1.0, 0.9,0,0.7 ) );	
	p->registerListener( c );
	p->registerListener( n );
	cube->addComponent( p );

	// Static Geometry
	EntityManager* entityManager = new EntityManager();
	StaticGeometry* staticGeometry = new StaticGeometry( entityManager, mRenderSystem, mPhysicsManager );

	staticGeometry->addGeometry( float3( 0,0,0), float3( 10.0f, 1.0f, 10.0f ), float3(0,0,0));

	// Camera
	Entity* camera = createEntity();

	CameraComponent* cc = new CameraComponent( mRenderSystem );
	PositionComponent* cpc = new PositionComponent();
	
	cpc->registerListener( cc );
	cc->registerListener( cpc );

	camera->addComponent(cc);
	camera->addComponent(cpc);

	cc->setPosition( float3(0,0.0f,40.0f) );
	cc->lookAt( float3(0,0,0) );

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
        case EV_KeyPress:
            KeyboardEvent* ke = static_cast<KeyboardEvent*>(e);
            switch (ke->mKeycode)
            {
				case SDL_SCANCODE_ESCAPE: // Escape
                   m_EngineShutdown = true;
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

