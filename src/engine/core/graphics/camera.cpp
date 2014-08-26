//
//  camera.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 27/05/2014.
//
//
#include "pch.h"
#include "camera.h"

Camera::Camera( Ogre::RenderWindow* rw, Ogre::SceneManager* sm, Ogre::SceneNode* rootNode )
{
    m_SceneMgr = sm;
    m_RenderWindow = rw;
    m_SceneNode = rootNode->createChildSceneNode();
}

void Camera::init()
{
    // TODO add multi-camera support
    m_Camera = m_SceneMgr->createCamera("Camera");

    m_Camera->setNearClipDistance(0.1f);
    m_Camera->setFarClipDistance(100.0f);

    // Set up viewport
    m_Viewport = m_RenderWindow->addViewport( m_Camera );
    m_Viewport->setBackgroundColour( Ogre::ColourValue(0.0f,0.0f,0.5f));

    m_Camera->setAspectRatio(Ogre::Real(m_Viewport->getActualWidth()) / Ogre::Real(m_Viewport->getActualHeight()));
    m_SceneNode->attachObject( m_Camera );
}
