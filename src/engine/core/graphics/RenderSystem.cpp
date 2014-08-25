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
    // The idea here is to open an SDL window *without* creating a GL context
    // and let Ogre create its own GL context, since it seems to be a little
    // sensitive when given an external one.
    // Ogre can be instructed to render to an external window, namely the one
    // SDL created.

    // Need an Ogre root (may want to log stuff from this point on, too)
    m_Root = new Ogre::Root("","","Ogre.log");

    // TODO get window width and height from somewhere (config file?)
    unsigned int windowWidth = 800;
    unsigned int windowHeight = 600;

	// Create SDL Window without OpenGL context
	SDL_Window* mSDLWindow = SDL_CreateWindow( "Window",
												SDL_WINDOWPOS_CENTERED,
												SDL_WINDOWPOS_CENTERED,
												windowWidth,
												windowHeight,
												SDL_WINDOWPOS_CENTERED|SDL_WINDOW_RESIZABLE // TODO Add window resize support
											);

    // Prepare Ogre render window parameters
    Ogre::NameValuePairList lParams;
    lParams["FSAA"] = "0";
    lParams["vsync"] = "true";
    lParams["macAPI"] = "carbon";

    // Get SDL window handle and insert into parameter list so ogre knows which
    // external window to use.
    SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo( mSDLWindow, &info );
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	size_t winHandle = reinterpret_cast<size_t>(info.info.win.window);
	lParams["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	size_t winHandle = reinterpret_cast<size_t>(info.info.x11.window);
	lParams["parentWindowHandle"] = Ogre::StringConverter::toString(winHandle);
#else
#   error Defined OGRE_PLATFORM not supported
#endif
    

    // Load the GL Rendersystem and set up
	Ogre::String renderer = "./RenderSystem_GL";
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 && defined(_DEBUG)
	renderer.append("_d");
#endif
    m_Root->loadPlugin( renderer );
    Ogre::RenderSystem* renderSystem = m_Root->getAvailableRenderers()[0];
    m_Root->setRenderSystem(renderSystem);

    // Initialise ogre to use the SDL window
    m_Root->initialise( false, "", "" );
    m_RenderWindow = m_Root->createRenderWindow( "Window", windowWidth, windowHeight, false, &lParams );

    // Create a default scene manager
    m_SceneMgr = m_Root->createSceneManager(Ogre::ST_GENERIC );

    // Cache root scene node
    m_RootSceneNode = m_SceneMgr->getRootSceneNode();
	
	// TODO initialize resource group
    Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Media");
}


RenderSystem::~RenderSystem()
{

	// Shut down ogre
	m_SceneMgr->clearScene();
	m_Root->destroySceneManager( m_SceneMgr );

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


Ogre::String RenderSystem::generateName(const Ogre::String& prefix /*= "Unnamed"*/)
{
	static std::map<Ogre::String, int> countMap;
	if (countMap.find(prefix) == countMap.end())
		countMap[prefix] = 0;
	return prefix + std::to_string(++countMap[prefix]);
}