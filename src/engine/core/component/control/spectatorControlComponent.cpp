#include "pch.h"
#include "engine/core/event/eventSystem.h"
#include "spectatorControlComponent.h"
#include <SDL.h>

static const float DEFAULT_SPECTATOR_SPEED = 5.0f;
static const float FAST_SPECTATOR_SPEED = 25.0f;

// Physics component could later be abstracted away - but it won't really work without it
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
	btSphereShape* shape = new btSphereShape( 0.1f );
	physicsComponent->initialise( shape, 0.4f, float3(0,0,0));
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
		case EV_CORE_KEY_PRESS:
			{			
				// Cast to keypress event
				KeyboardEvent* ke = e->getData<KeyboardEvent>();
				switch( ke->mKeycode )
				{
				case SDL_SCANCODE_W:
						mForward = true;
					break;
				case SDL_SCANCODE_A:
						mLeft = true;
					break;
				case SDL_SCANCODE_D:
						mRight = true;
					break;
				case SDL_SCANCODE_S:
						mReverse = true;

					break;
				// Speed modifier
				case SDL_SCANCODE_LSHIFT:
						mSpeed = FAST_SPECTATOR_SPEED;
				}
			}
		break;

		case EV_CORE_KEY_RELEASE:
			{
				KeyboardEvent* ke = e->getData<KeyboardEvent>();
				switch( ke->mKeycode )
				{
				case SDL_SCANCODE_W:
					mForward = false;
					break;
				case SDL_SCANCODE_A:
					mLeft = false;
					break;
				case SDL_SCANCODE_D:
					mRight = false;
					break;
				case SDL_SCANCODE_S:
					mReverse = false;

					break;
					// Speed modifier
				case SDL_SCANCODE_LSHIFT:
					mSpeed = DEFAULT_SPECTATOR_SPEED;
				}
			}
		break;

		case EV_CORE_MOUSE_MOVEMENT:
			{
				MouseEvent* me = e->getData<MouseEvent>();
				
				// Update internal angles
				xAng = xAng*Quat::RotateX( -me->mMouseMoveY/1000.0  );
				yAng = yAng*Quat::RotateY( -me->mMouseMoveX/1000.0);

				btTransform initial = mPhysicsComponent->getBody()->getWorldTransform();
				btTransform updated( yAng * xAng,initial.getOrigin() );
				mPhysicsComponent->getBody()->setWorldTransform( updated );
				mPhysicsComponent->getBody()->activate(true);
			}

	}
}

void SpectatorControlComponent::update(double dt)
{
	float3 dir(0,0,0);
	if( mForward )
		dir+=float3( 0,0,-mSpeed );
	if( mReverse )
		dir+=float3( 0,0,mSpeed );
	if( mLeft )
		dir+=float3( -mSpeed,0,0 );
	if( mRight )
		dir+=float3(mSpeed,0,0);

	Event* e = EventSystem::getSingletonPtr()->getEvent(EV_CORE_APPLY_FORCE);
	TransformEvent* te = e->getData<TransformEvent>();
	te->mFloat3_1 = dir;

	dispatch(e);

	EventSystem::getSingletonPtr()->releaseEvent(e);
}

void SpectatorControlComponent::applyForceInDir( float3 dir )
{
	mPhysicsComponent->getBody()->applyCentralForce(  Quat(mPhysicsComponent->getBody()->getWorldTransform().getRotation()).Transform( dir ) );
}

