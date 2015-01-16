#ifndef cameraManager_h__
#define cameraManager_h__

#include "renderSystem.h"
#include "../../component/render/cameraComponent.h"

class CameraManager: public ComponentManager<CameraComponent>, public EventListener, public Manager
{
public:
	CameraManager( RenderSystem* rend );
	~CameraManager();

	void createNewCamera( CameraComponent* comp );
	void setCameraOffset( CameraComponent* comp, float3 offset );
	void lookAt( CameraComponent* comp, float3 pos );

	Ogre::Viewport* getViewport();

	void handle(Event* e);
	void update( double dt );
private:

	RenderSystem* mRenderSystem;

	// Multiple Viewport and Camera support
	std::map<Ogre::Camera*,Ogre::Viewport*>    m_CameraMap;
};
#endif // cameraManager_h__
