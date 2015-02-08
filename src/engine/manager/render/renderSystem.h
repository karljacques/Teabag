#ifndef  __Teabag__Rendersystem__
#define __Teabag__Rendersystem__

#include "pch.h"
#include "../../core/entity-component/componentManager.h"
#include "../../component/render/renderComponent.h"
#include "../../core/entity-component/entityManager.h"
#include "../../component/physics/physicsComponent.h"
#include "../manager.h"

class RenderManager : public ComponentManager, public EventListener, public Manager
{
public:
        
    RenderManager( );
    ~RenderManager();
        
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

	// Component Methods
	void initComponent( RenderComponent* comp );
	void setAsBox( RenderComponent* comp, float3 dim );

	void handle( Event* e );
	void update( double dt );

private:
        
    // Core
    std::unique_ptr<Ogre::Root>		m_Root;
    Ogre::SceneManager*				m_SceneMgr;
    Ogre::RenderWindow*				m_RenderWindow;
    Ogre::SceneNode*				m_RootSceneNode;
        
    // Window
    size_t          m_WindowHandle;
	SDL_Window*			  mWindow;

};

#endif /* defined( __Teabag__Rendersystem__) */
