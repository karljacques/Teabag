#ifndef renderSystem_h__
#define renderSystem_h__

#include "pch.h"
#include "common.h"

namespace render
{
	void init( void );
	void destroy( void );

	// Getters
	Ogre::Root*           getOgreRoot( void );
	Ogre::SceneManager*   getSceneMgr( void );
	Ogre::RenderWindow*   getRenderWindow( void );
	Ogre::SceneNode*      getRootSceneNode( void );
	SDL_Window*			  getSDLWindow( void );

	void            update( void );
	std::string		generateName( const std::string& prefix = "Unnamed" );
}

#endif // renderSystem_h__
