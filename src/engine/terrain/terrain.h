//
//  terrain.h
//  YorkshireTea
//
//  Created by Karl Jacques on 29/05/2014.
//
//

#ifndef __YorkshireTea__terrain__
#define __YorkshireTea__terrain__

#include "pch.h"
#include <iostream>
#include "sector.h"

class Terrain
{
public:
    
    Terrain( Ogre::SceneManager* SceneMgr );
    Sector* getRootSector();
    int height[1000][1000];
    
    void construct();
private:
    
    Sector* mRootSector;
    
    Ogre::MeshPtr mMesh;
    Ogre::SubMesh* mSubMesh;
    
    Ogre::VertexDeclaration* mDecl;
    Ogre::HardwareVertexBufferSharedPtr mVbuf;
    
    void _setupVertexBuffer();
    
};



#endif /* defined(__YorkshireTea__terrain__) */
