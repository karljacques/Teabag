//
//  osxEngine.m
//  YorkshireTea
//
//  Created by Karl Jacques on 26/05/2014.
//
//
#include "pch.h"
#include <SDL.h>

#include "Engine.h"


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
	cube->renderComponent =  new RenderComponent( mRenderSystem, cube );
	cube->renderComponent->setAsBox(1.0f,1.0f,1.0f);

	cube->physicsComponent = new PhysicsComponent( mPhysicsManager, cube );
	cube->physicsComponent->setAsBox( 1.0f, 1.0f, 1.0f );

	Entity* floor = createEntity();
	floor->renderComponent =  new RenderComponent( mRenderSystem, cube );
	floor->mPosition = Ogre::Vector3( 0,0,0 );
	floor->renderComponent->setAsBox(10.0f,0.5f,10.0f);
	

}

Engine::~Engine()
{
	SDL_Quit();

	delete mRenderSystem;
	delete mInputSystem;
	delete mEventSystem;

}
void Engine::update()
{
	// Calculate timestep
	double dt = mTimeSinceLastUpdate.getMilliseconds()/1000.0;
	mTimeSinceLastUpdate.reset();

	// Update systems and managers
    mRenderSystem->renderOneFrame();
    mInputSystem->update();
    mEventSystem->handleEvents();
	mPhysicsManager->update( dt );

	// Update game entities
	for( auto i = mEntities.begin(); i != mEntities.end(); i++ )
		(*i)->update( dt );

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
            switch (ke->m_Keycode)
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

