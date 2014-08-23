//
//  camera.h
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//

#ifndef __YorkshireTea__camera__
#define __YorkshireTea__camera__

#include <iostream>

#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "RenderSystem.h"

class Camera
{
public:
    Camera( Ogre::RenderWindow* rw, Ogre::SceneManager* sm , Ogre::SceneNode* rootNode );
        
    void init( );
        

    Ogre::Viewport*     m_Viewport;
    Ogre::Camera*       m_Camera;
    Ogre::SceneNode*    m_SceneNode;
        
    // Dependency
    Ogre::SceneManager* m_SceneMgr;
    Ogre::RenderWindow* m_RenderWindow;
};

#endif /* defined(__YorkshireTea__camera__) */
