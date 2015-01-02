#include "pch.h"
#include "spectatorManager.h"

#define SPECTATOR_DEFAULT_SPEED 20.0f

void SpectatorManager::handle( Event* e )
{
	/* Transform key presses into directions */
	if( e->getEventType() == EV_CORE_KEY_PRESS )
	{
		// Cast to key press
		KeyboardEvent* k = e->getData<KeyboardEvent>();

		/* Loop through spectators and apply */
		for( auto i=mComponents.begin(); i!=mComponents.end();i++ )
		{
			SpectatorComponent* s = i->second;

			switch( k->mKeycode )
			{
			case SDL_SCANCODE_W:
				s->forward = true;
				break;
			case SDL_SCANCODE_A:
				s->left = true;
				break;
			case SDL_SCANCODE_S:
				s->backward = true;
				break;
			case SDL_SCANCODE_D:
				s->right = true;
				break;
			}

		}
	}

	if( e->getEventType() == EV_CORE_KEY_RELEASE )
	{
		KeyboardEvent* k = e->getData<KeyboardEvent>();

		for( auto i=mComponents.begin(); i!=mComponents.end();i++)
		{
			SpectatorComponent* s = i->second;

			switch( k->mKeycode )
			{
			case SDL_SCANCODE_W:
				s->forward = false;
				break;
			case SDL_SCANCODE_A:
				s->left = false;
				break;
			case SDL_SCANCODE_S:
				s->backward = false;
				break;
			case SDL_SCANCODE_D:
				s->right = false;
				break;
			}
		}
	}
	/* Handle mouse movement */
	if( e->getEventType() == EV_CORE_MOUSE_MOVEMENT )
	{
		MouseEvent* me = e->getData<MouseEvent>();
		
		for( auto i=mComponents.begin(); i!=mComponents.end();i++ )
		{
			SpectatorComponent s = *i->second;
			s.xAng = s.xAng*Quat::RotateX( -me->mMouseMoveY/1000.0f );
			s.yAng = s.yAng*Quat::RotateY( -me->mMouseMoveX/1000.0f );

			// Get the physics component - set new orientation
			PhysicsComponent* phys = mEntityManager->getByLUID(s.LUID)->getComponent<PhysicsComponent>();
			phys->body->setWorldTransform( btTransform( s.yAng*s.xAng , phys->body->getWorldTransform().getOrigin()));
			phys->body->activate((true));
		}
		
	}
}

SpectatorManager::SpectatorManager(EntityManager* ent)
{
	mEntityManager = ent;
}

void SpectatorManager::update()
{
	for( auto i = mComponents.begin(); i!=mComponents.end(); i++ )
	{
		SpectatorComponent* s = i->second;

		float3 dir(0,0,0);

		if( s->forward )
			dir+= float3(0,0,-1);
		if( s->backward )
			dir+= float3(0,0,1);
		if( s->right )
			dir+= float3(1,0,0);
		if( s->left )
			dir+= float3(-1,0,0);

		if( dir.Length()>0)
		{
			// Get physics component
			PhysicsComponent* phys = mEntityManager->getByLUID(s->LUID)->getComponent<PhysicsComponent>();
			phys->body->applyCentralForce( dir*SPECTATOR_DEFAULT_SPEED  );
			phys->body->activate(true);
		}
		
	
	}
}