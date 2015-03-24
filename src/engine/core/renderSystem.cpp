#include "pch.h"
#include "renderSystem.h"

// Core
static Ogre::Root*						m_Root;
static Ogre::SceneManager*				m_SceneMgr;
static Ogre::RenderWindow*				m_RenderWindow;
static Ogre::SceneNode*				m_RootSceneNode;

// Window
static size_t          m_WindowHandle;
static SDL_Window*		m_Window;

void render::init( void )
{
	// The idea here is to open an SDL window *without* creating a GL context
	// and let Ogre create its own GL context, since it seems to be a little
	// sensitive when given an external one.
	// Ogre can be instructed to render to an external window, namely the one
	// SDL created.

	// Need an Ogre root (may want to log stuff from this point on, too)
	m_Root = new Ogre::Root("","","Ogre.log");

	// TODO get window width and height from somewhere (config file?)
	unsigned int windowWidth = 1280;
	unsigned int windowHeight = 720;

	// Create SDL Window without OpenGL context
	m_Window = SDL_CreateWindow( "Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOWPOS_CENTERED // TODO Add window resize support
		);

	// Prepare Ogre render window parameters
	Ogre::NameValuePairList lParams;
	lParams["FSAA"] = "0";
	lParams["vsync"] = "false";

	// Get SDL window handle and insert into parameter list so ogre knows which
	// external window to use.
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo( m_Window, &info );
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
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media","FileSystem");
	Ogre::ResourceGroupManager::getSingleton().declareResource("dejavu.gorilla", "Material");
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void render::destroy( void )
{
	// Shut down ogre
	m_SceneMgr->clearScene();
	m_Root->destroySceneManager( m_SceneMgr );

	delete m_Root;
}

Ogre::Root* render::getOgreRoot(void)
{
	return m_Root;
}

Ogre::SceneManager* render::getSceneMgr(void)
{
	return m_SceneMgr;
}

Ogre::RenderWindow* render::getRenderWindow(void)
{
	return m_RenderWindow;
}

Ogre::SceneNode* render::getRootSceneNode(void)
{
	return m_RootSceneNode;
}

SDL_Window* render::getSDLWindow(void)
{
	return m_Window;
}

void render::update(void)
{
	m_Root->renderOneFrame();

	// Make sure to pump messages in all render windows
	Ogre::WindowEventUtilities::messagePump();
}

std::string render::generateName(const std::string& prefix /*= "Unnamed" */)
{
	static std::map<Ogre::String, int> countMap;
	if (countMap.find(prefix) == countMap.end())
		countMap[prefix] = 0;
	return prefix + std::to_string(++countMap[prefix]);
}






