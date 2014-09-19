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
	mNetworkSystem = new ServerNetworkSystem();
	
	// Register the engine to receive input events
    this->setEventType(EV_CORE_KEY_PRESS||EV_CORE_KEY_RELEASE );
    mEventSystem->registerListener( this );

	// Create the default camera
	PositionComponent* defaultCameraPos = new PositionComponent();
	CameraComponent* defaultCamera = new CameraComponent( mRenderSystem,defaultCameraPos );

	// Create console - Singleton
	new OgreConsole(this);
	OgreConsole::getSingleton().addCommand( "net.connect", &Console_Net_Connect );

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

