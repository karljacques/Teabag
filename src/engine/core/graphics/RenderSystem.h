//
//  gfxSystem.h
//  YorkshireTea
//
//  Created by Karl Jacques on 24/05/2014.
//
//

#ifndef __YorkshireTea__gfxSystem__
#define __YorkshireTea__gfxSystem__

#include "Camera.h"

class Camera;
    
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
        
    std::vector<Camera*>*    getCameraList();
        
    // Functions
    void            renderOneFrame();
   
private:
        
    // Core
    Ogre::Root*           m_Root;
    Ogre::SceneManager*   m_SceneMgr;
    Ogre::RenderWindow*   m_RenderWindow;
    Ogre::SceneNode*      m_RootSceneNode;
        
    // Mutliple Viewport and Camera support
    std::vector<Camera*>    m_CameraList;
        
        
    // Window
    size_t          m_WindowHandle;
};

#endif /* defined(__YorkshireTea__gfxSystem__) */
