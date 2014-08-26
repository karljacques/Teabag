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
#include "engine/core/component/physics/physicsComponent.h"
#include "engine/core/component/render/renderComponent.h"
#include "engine/core/component/base/positionComponent.h"

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
	RenderComponent* c =  new RenderComponent( mRenderSystem, cube );
	c->setAsBox(1.0f,1.0f,1.0f);
	cube->addComponent(c);

	PositionComponent* n = new PositionComponent();
	cube->addComponent( n );
	n->registerListener( c );
	
	PhysicsComponent* p = new PhysicsComponent( mPhysicsManager, cube );
	btCollisionShape* shapex = new btBoxShape( float3(1.0f,1.0f, 1.0f ) );
	p->initialise( shapex, 5.0, float3(0,50,0), Quat( 1.0, 0.9,0,0.2 ) );	
	p->registerListener( c );
	p->registerListener( n );
	cube->addComponent( p );

	// floor
	Entity* floor = createEntity();
	RenderComponent* r = new RenderComponent( mRenderSystem, floor );
	r->setAsBox( 100.0f, 0.01f, 100.0f );
	floor->addComponent( r );

	PhysicsComponent* pc = new PhysicsComponent( mPhysicsManager, floor );
	btCollisionShape* shape = new btBoxShape( float3(10.0f,0.1f, 10.0f ) );
	pc->initialise( shape, 0, float3(0,0,0) );
	floor->addComponent( pc );

	pc->registerListener( r );

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

