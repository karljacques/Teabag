//
//  terrain.cpp
//  YorkshireTea
//
//  Created by Karl Jacques on 29/05/2014.
//
//
#include "pch.h"
#include "terrain.h"


Terrain::Terrain( Ogre::SceneManager* SceneMgr )
{
    mRootSector = new Sector();
    
    for( int i=0; i < 1000; i++ )
        for( int j=0; j < 1000; j++ )
            height[i][j] = 1;
    
    _setupVertexBuffer();
    construct();
    
    Ogre::Entity* thisEntity = SceneMgr->createEntity("c","TerrainMesh");
    SceneMgr->getRootSceneNode()->attachObject(thisEntity);
    
}

Sector* Terrain::getRootSector()
{
    return mRootSector;
}

void Terrain::construct()
{
    
}
void Terrain::_setupVertexBuffer()
{
    mMesh = Ogre::MeshManager::getSingleton().createManual("TerrainMesh","Media");
    mSubMesh = mMesh->createSubMesh();
    
    mMesh->sharedVertexData = new Ogre::VertexData;
    mMesh->sharedVertexData->vertexCount = 100*100;
    
    // create vertex buffer structure
    mDecl = mMesh->sharedVertexData->vertexDeclaration;
    size_t offset = 0;
    
    mDecl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
    offset+= Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
    
    mVbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(offset, 100*100, Ogre::HardwareBuffer::HBU_STATIC );
    
    float* vertices = static_cast<float*>(mVbuf->lock(Ogre::HardwareBuffer::HBL_NORMAL));
    
    int k = 0;
    for( int i=0; i<100; i++ )
        for( int j=0;j<100;j++ )
        {
            vertices[k] = i;
            vertices[k+2] = j;
            vertices[k+1] = height[i][j];
            k+=3;
        }
    
    mVbuf->unlock();
    
    /* create the index buffer */
    Ogre::HardwareIndexBufferSharedPtr indexBuffer = Ogre::HardwareBufferManager::getSingleton().
    createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, mMesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC);
    
    /* lock the buffer so we can get exclusive access to its data */
    uint16_t *indices = static_cast<uint16_t *>(indexBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));
    
    /* define our triangle */
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    
    /* unlock the buffer */
    indexBuffer->unlock();
    
    mMesh->sharedVertexData->vertexBufferBinding->setBinding(0, mVbuf);
    mSubMesh->useSharedVertices = true;
    mSubMesh->indexData->indexBuffer = indexBuffer;
    mSubMesh->indexData->indexCount = mMesh->sharedVertexData->vertexCount;
    mSubMesh->indexData->indexStart = 0;
    
    mMesh->_setBounds(Ogre::AxisAlignedBox(-100,-100,-100,100,100,100 ));
    
    //mMesh->load();
    
}