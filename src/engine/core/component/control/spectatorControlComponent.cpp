#include "pch.h"
#include "spectatorControlComponent.h"
#include <SDL.h>

SpectatorControlComponent::SpectatorControlComponent( PhysicsComponent* physicsComponent )
{
	// Initial Variables
	mPhysicsComponent = physicsComponent;
	mForward = false;

	// Create collision sphere here
	btSphereShape* shape = new btSphereShape( 1.0f );
	physicsComponent->initialise( shape, 1.0f, float3(0,0,0));
	physicsComponent->getBody()->setGravity( float3(0,0,0));
}


SpectatorControlComponent::~SpectatorControlComponent(void)
{
}

void SpectatorControlComponent::handle( Event* e )
{
	switch( e->getEventType() )
	{
		case EV_KEY_PRESS:
			{

			// Cast to keypress event
			KeyboardEvent* ke = static_cast<KeyboardEvent*>(e);
			if( ke->mKeycode == SDL_SCANCODE_W )
				if( ke->mPressed == true )
					mForward = true;
				else if(ke->mReleased == true)
					mForward = false;
			}
		break;

		case EV_MOUSEMOVE:
			{

			}

	}
}

void SpectatorControlComponent::update(double dt)
{
	if( mForward )
		mPhysicsComponent->getBody()->applyCentralForce(  float3(0,0,-10));
}

