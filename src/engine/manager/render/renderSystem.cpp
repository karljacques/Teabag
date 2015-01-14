//
//  gfxSystem.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//

#include "pch.h"
#include "renderSystem.h"
#include <SDL.h>
#include <SDL_syswm.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "OSXUtils.h"
#endif

RenderSystem::RenderSystem( EntityManager* ent )
{

	// Dependency injection
	mEntityMgr = ent;

    // The idea here is to open an SDL window *without* creating a GL context
    // and let Ogre create its own GL context, since it seems to be a little
    // sensitive when given an external one.
    // Ogre can be instructed to render to an external window, namely the one
    // SDL created.

    // Need an Ogre root (may want to log stuff from this point on, too)
    m_Root = new Ogre::Root("","","Ogre.log");

    // TODO get window width and height from somewhere (config file?)
    unsigned int windowWidth = 1440;
    unsigned int windowHeight = 900;

	// Create SDL Window without OpenGL context
	mWindow = SDL_CreateWindow( "Window",
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

    // Get SDL window handle and insert into parameter list so ogre knows which
    // external window to use.
    SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo( mWindow, &info );
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	size_t winHandle = reinterpret_cast<size_t>(info.info.win.window);
	lParams["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	size_t winHandle = reinterpret_cast<size_t>(info.info.x11.window);
	lParams["parentWindowHandle"] = Ogre::StringConverter::toString(winHandle);
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	lParams["macAPI"] = "cocoa";
	lParams["macAPICocoaUseNSView"] = "true";
	lParams["externalWindowHandle"] = Ogre::StringConverter::toString(
		getNSViewFromNSWindow(reinterpret_cast<size_t>(info.info.cocoa.window)));
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
	
	// Load resources
   // Ogre::ResourceGroupManager::getSingleton().createResourceGroup("General");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media","FileSystem");
	Ogre::ResourceGroupManager::getSingleton().declareResource("dejavu.gorilla", "Material");
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
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


void RenderSystem::initComponent( RenderComponent* comp )
{
	comp->mSceneNode = getRootSceneNode()->createChildSceneNode();
}

void RenderSystem::setAsBox( RenderComponent* comp, float3 dim )

{
	Ogre::ManualObject* cube = new Ogre::ManualObject("Cube");
	cube->begin("BaseWhiteNoLighting");

	float x = dim.x/2.0;
	float y = dim.y/2.0;
	float z = dim.z/2.0;

	float c1 = rand() %255;
	float c2 = rand() %255;
	float c3 = rand() %255;

	// Random-ish colours
	cube->position( x,y,-z ); // v0
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position(-x,y,-z ); // v1 
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position(-x,-y,-z); // v2
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position( x,-y,-z); // v3 
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position( x,-y,z ); // v4

	cube->colour( x*c1,y*c2,z*c3 );
	cube->position( x,y,z  ); // v5
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position( -x,y,z ); // v6
	cube->colour( x*c1,y*c2,z*c3 );
	cube->position( -x,-y,z); // v7
	cube->colour( x*c1,y*c2,z*c3 );

	cube->quad( 0,1,2,3 );
	cube->quad( 5,6,1,0 );
	cube->quad( 6,5,4,7 );
	cube->quad( 1,6,7,2 );
	cube->quad( 5,0,3,4 );
	cube->quad( 3,2,7,4 );

	cube->end(); 

	comp->mObject = cube;
	comp->mSceneNode->attachObject(comp->mObject);
}

void RenderSystem::handle( Event* e )
{
	if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
	{
		if( componentExists(e->ID ))
		{
			RenderComponent* comp = getComponentByID( e->ID );
			TransformEvent* te = e->getData<TransformEvent>();
			comp->mSceneNode->setPosition( te->position );
			comp->mSceneNode->setOrientation( te->orientation );

		}

	}
}
