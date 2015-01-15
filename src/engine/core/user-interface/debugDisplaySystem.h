#ifndef debugDisplaySystem_h__
#define debugDisplaySystem_h__

#include "../event/eventListener.h"
#include "../render/UI/Gorilla.h"
#include "../../manager/render/cameraManager.h"

class DebugDisplaySystem : public EventListener
{
public:
	DebugDisplaySystem(CameraManager* mCameraManager );
	~DebugDisplaySystem(){};
	void handle(  Event* e  );
	void update( double dt );
private:

	// Gorilla screens
	Gorilla::Screen* mScreen;
	Gorilla::Layer* mLayer;
	Gorilla::MarkupText* mTextDisplay;

	// Debugging trackers
	std::list<int> mFPSRecorder;
};
#endif // debugDisplaySystem_h__
