#include "pch.h"
#include "spectatorControlComponent.h"
#include <SDL.h>

static const float DEFAULT_SPECTATOR_SPEED = 10.0f;
static const float FAST_SPECTATOR_SPEED = 20.0f;

SpectatorControlComponent::SpectatorControlComponent( PhysicsComponent* physicsComponent )
{
	// Initial Variables
	mPhysicsComponent = physicsComponent;

	mForward = false;
	mLeft = false;
	mRight = false;
	mReverse = false;

	mSpeed = DEFAULT_SPECTATOR_SPEED;

	xAng = Quat(0,0,0,1);
	yAng = Quat(0,0,0,1);

	// Create collision sphere here
	btSphereShape* shape = new btSphereShape( 1.0f );
	physicsComponent->initialise( shape, 1.0f, float3(0,0,0));
	physicsComponent->getBody()->setGravity( float3(0,0,0));
	mPhysicsComponent->getBody()->setDamping( 0.9, 1.0f );

	mPhysicsComponent->getBody()->setAngularFactor(btVector3(1.0f,1.0f,0.0f));
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
				switch( ke->mKeycode )
				{
				case SDL_SCANCODE_W:
					if( ke->mPressed == true )
						mForward = true;
					else if(ke->mReleased == true)
						mForward = false;
					break;
				case SDL_SCANCODE_A:
					if( ke->mPressed)
						mLeft = true;
					else if( ke->mReleased )
						mLeft = false;
					break;
				case SDL_SCANCODE_D:
					if( ke->mPressed )
						mRight = true;
					else if( ke->mReleased )
						mRight = false;
					break;
				case SDL_SCANCODE_S:
					if( ke->mPressed )
						mReverse = true;
					else if(ke->mReleased)
						mReverse = false;
					break;
				// Speed modifier
				case SDL_SCANCODE_LSHIFT:
					if( ke->mPressed )
						mSpeed = FAST_SPECTATOR_SPEED;
					else if(ke->mReleased)
						mSpeed = DEFAULT_SPECTATOR_SPEED;
				}
			}
		break;

		case EV_MOUSE_MOVEMENT:
			{
				MouseEvent* me = static_cast<MouseEvent*>(e);
				
				// Update internal angles
				xAng = xAng*Quat::RotateX( -me->mMouseMoveY/1000.0  );
				yAng = yAng*Quat::RotateY( -me->mMouseMoveX/1000.0);

				btTransform initial = mPhysicsComponent->getBody()->getWorldTransform();
				btTransform updated( yAng * xAng,initial.getOrigin() );
				mPhysicsComponent->getBody()->setWorldTransform( updated );
				mPhysicsComponent->getBody()->activate(true);
				//mPhysicsComponent->update( 0 );
			}

	}
}

void SpectatorControlComponent::update(double dt)
{
	if( mForward )
		applyForceInDir( float3( 0,0,-mSpeed ) );
	if( mReverse )
		applyForceInDir( float3( 0,0,mSpeed ) );
	if( mLeft )
		applyForceInDir( float3( -mSpeed,0,0 ) );
	if( mRight )
		applyForceInDir( float3(mSpeed,0,0));
}

void SpectatorControlComponent::applyForceInDir( float3 dir )
{
	mPhysicsComponent->getBody()->applyCentralForce(  Quat(mPhysicsComponent->getBody()->getWorldTransform().getRotation()).Transform( dir ) );
}

