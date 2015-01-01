#ifndef cameraManager_h__
#define cameraManager_h__

#include "..\component\componentManager.h"
#include "..\component\render\cameraComponent.h"
#include "renderSystem.h"

class CameraManager: public ComponentManager<CameraComponent>, public EventListener
{
	CameraManager( RenderSystem* rend );
	~CameraManager();

	void createNewCamera( CameraComponent* comp );
	void setCameraOffset( CameraComponent* comp, float3 offset );
	void lookAt( CameraComponent* comp, float3 pos );

	void handle(Event* e);
private:

	RenderSystem* mRenderSystem;
};
#endif // cameraManager_h__
