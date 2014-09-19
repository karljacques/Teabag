//
//  gfxSystem.h
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//

#ifndef  __Teabag__Rendersystem__
#define __Teabag__Rendersystem__

#include "pch.h"
#include <SDL.h>

class RenderSystem
{
public:
        
    RenderSystem();
    ~RenderSystem();
        
    // Getters
    Ogre::Root*           getOgreRoot();
    Ogre::SceneManager*   getSceneMgr();
    Ogre::RenderWindow*   getRenderWindow();
    Ogre::SceneNode*      getRootSceneNode();
	SDL_Window*			  getSDLWindow(){ return mWindow; }
    Ogre::Viewport*	      getViewport( );

    // Functions
    void            renderOneFrame();
	Ogre::String	generateName(const Ogre::String& prefix = "Unnamed" );
	
	// Camera methods
	void addCameraPair( Ogre::Camera* c, Ogre::Viewport* v);

private:
        
    // Core
    Ogre::Root*           m_Root;
    Ogre::SceneManager*   m_SceneMgr;
    Ogre::RenderWindow*   m_RenderWindow;
    Ogre::SceneNode*      m_RootSceneNode;

    // Multiple Viewport and Camera support
    std::map<Ogre::Camera*,Ogre::Viewport*>    m_CameraMap;
        
    // Window
    size_t          m_WindowHandle;
	SDL_Window*			  mWindow;
};

#endif /* defined( __Teabag__Rendersystem__) */
