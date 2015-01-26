#include "pch.h"
#include "debugDisplaySystem.h"
#include "common.h"

#define CONSOLE_FONT_INDEX 9

DebugDisplaySystem::DebugDisplaySystem( CameraManager* mCameraManager )
{
	mScreen = Gorilla::Silverback::getSingletonPtr()->createScreen( mCameraManager->getViewport(), "dejavu" );
	mLayer = mScreen->createLayer(15);

	mTextDisplay = mLayer->createMarkupText(CONSOLE_FONT_INDEX, mScreen->getWidth() - 50, 10, Ogre::StringUtil::BLANK );

}

void DebugDisplaySystem::handle( Event* e )
{

}

void DebugDisplaySystem::update( double dt )
{
	int FPS = (int)1.0f/(dt*MICROSECONDS_TO_SECONDS_FACTOR);

	mFPSRecorder.push_back(FPS);
	if( mFPSRecorder.size() > 60 )
	{
		mFPSRecorder.pop_front();
        
	}

	// Calculate average fps
	int cumFPS = 0;
	for( auto i=mFPSRecorder.begin(); i!=mFPSRecorder.end(); i++ )
	{
		cumFPS += *i;
	}
	FPS = cumFPS/mFPSRecorder.size();

	mTextDisplay->text("FPS: " + std::to_string(FPS));
}

