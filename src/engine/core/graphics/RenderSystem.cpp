//
//  gfxSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//

#include "pch.h"
#include "RenderSystem.h"
#include <SDL.h>
#include <SDL_syswm.h>

RenderSystem::RenderSystem()
{
    m_Root = new Ogre::Root("","","Ogre.log");

    // Load the GL Rendersystem and set up
	Ogre::String renderer = "RenderSystem_GL";

    // TheComet: *nix requires a relative path
#ifndef WIN32
    renderer = Ogre::String("./") + renderer;
#endif

#if defined(_DEBUG) && defined(WIN32)
	renderer.append("_d");
#endif

    m_Root->loadPlugin( renderer );

    Ogre::RenderSystem* renderSystem = m_Root->getAvailableRenderers()[0];

    m_Root->setRenderSystem(renderSystem);

    Ogre::NameValuePairList lParams;

    lParams["FSAA"] = "0";
    lParams["vsync"] = "true";
    lParams["macAPI"] = "carbon";

	// Create SDL Window
	SDL_Window* mSDLWindow = SDL_CreateWindow( "Window",
												SDL_WINDOWPOS_CENTERED,
												SDL_WINDOWPOS_CENTERED,
												800,600,
												SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE // TODO Add window resize support
											);

#ifdef __WINDOWS__
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo( mSDLWindow, &info );

	size_t winHandle = reinterpret_cast<size_t>(info.info.win.window);
	lParams["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);

#else
	lParams["currentGLContext"] = Ogre::String("True");
	size_t winHandle = reinterpret_cast<size_t>(info.info.x11.window);
	lParams["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);
#endif

    m_Root->initialise( false, "", "" );
    m_RenderWindow = m_Root->createRenderWindow( "Window", 800, 600, false, &lParams );

    m_SceneMgr = m_Root->createSceneManager(Ogre::ST_GENERIC );

    m_RootSceneNode = m_SceneMgr->getRootSceneNode();

	// TODO initialise resource group
    Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Media");

	m_SceneMgr->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));
}


RenderSystem::~RenderSystem()
{
    delete m_Root;
}

Ogre::Root* RenderSystem::getOgreRoot()
{
    return m_Root;
}

Ogre::SceneManager* RenderSystem::getSceneMgr()
{
    return m_SceneMgr;
}

Ogre::RenderWindow* RenderSystem::getRenderWindow()
{
    return m_RenderWindow;
}

Ogre::SceneNode* RenderSystem::getRootSceneNode()
{
    return m_RootSceneNode;
}

std::vector<Camera*>* RenderSystem::getCameraList()
{
    return &m_CameraList;
}

void RenderSystem::renderOneFrame()
{
    m_Root->renderOneFrame();
}


