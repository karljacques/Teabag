#ifndef cameraManager_h__
#define cameraManager_h__

#include "renderSystem.h"
#include "../../component/render/cameraComponent.h"

class CameraManager: public ComponentManager, public EventListener, public Manager
{
public:
	CameraManager( RenderSystem* rend );
	~CameraManager();

	void createNewCamera( CameraComponent* comp );
	void setCameraOffset( CameraComponent* comp, float3 offset );
	void lookAt( CameraComponent* comp, float3 pos );

	// Copies a camera rather than creating a new one.
	// This is due to the fact that we need a default camera, which should be copied
	// We only need to create a new camera for multiple rendering, which at this point is not required.
	// So really, there should be one camera component - plus the default camera, which it is copied from.
	void getCamera( CameraComponent* comp );

	Ogre::Viewport* getViewport();

	void handle(Event* e);
	void update( double dt );
private:

	RenderSystem* mRenderSystem;

	// Multiple Viewport and Camera support
	std::map<Ogre::Camera*,Ogre::Viewport*>    m_CameraMap;
};
#endif // cameraManager_h__
