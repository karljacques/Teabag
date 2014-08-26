//
//  main.cpp
//  yorkshire-tea 17.31.42
//
//  Created by Karl Jacques on 23/05/2014.
//  Copyright (c) 2014 Karl Jacques. All rights reserved.
//
#include "pch.h"
#include "main.h"
#include "engine/engine.h"

#ifdef __cplusplus
extern "C"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main()
#endif
{
    Engine* engine = new Engine();
    engine->createCamera();

	Camera* cam = engine->getGfxSystem()->getCameraList()->front();
	cam->m_SceneNode->setPosition( 0.0f,2.0f, 20.0f );
	cam->m_Camera->lookAt( 0,0.0f,0.0f );

    while( !engine->isShuttingDown() )
    {
        engine->update();
    }

    delete engine;
	return 0;
}
