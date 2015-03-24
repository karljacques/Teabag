#include "pch.h"
#include "spectatorManager.h"


#define SPECTATOR_DEFAULT_SPEED 120.0f
#define SPECTATOR_BOOST_MULTIPLIER 5.0f

void SpectatorManager::handle( Event* e )
{
	/* Transform key presses into directions */
	if( e->getEventID() == EV_CORE_KEY_PRESS )
	{
		// Cast to key press
		KeyboardEvent* k = e->getData<KeyboardEvent>();

		/* Loop through spectators and apply */
		for( auto i=mComponents.begin(); i!=mComponents.end();i++ )
		{
			SpectatorComponent* s = dynamic_cast<SpectatorComponent*>(i->second);
			assert( s!=nullptr);
			if( entitysys::getByID(s->ID)->getComponent<PlayerComponent>()->pGUID == network::getPlayerGUID() )
			{
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
				case SDL_SCANCODE_LSHIFT:
					s->boost = true;
					break;
				}
			}
		}
	}

	if( e->getEventID() == EV_CORE_KEY_RELEASE )
	{
		KeyboardEvent* k = e->getData<KeyboardEvent>();

		for( auto i=mComponents.begin(); i!=mComponents.end();i++)
		{
			SpectatorComponent* s = dynamic_cast<SpectatorComponent*>(i->second);
			assert( s!=nullptr );
			if( entitysys::getByID(s->ID)->getComponent<PlayerComponent>()->pGUID == network::getPlayerGUID() )
			{
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
				case SDL_SCANCODE_LSHIFT:
					s->boost = false;
					break;
				}
			}
		}
	}
	/* Handle mouse movement */
	if( e->getEventID() == EV_CORE_MOUSE_MOVEMENT )
	{
		MouseEvent* me = e->getData<MouseEvent>();
		
		for( auto i=mComponents.begin(); i!=mComponents.end();i++ )
		{
			SpectatorComponent* s = dynamic_cast<SpectatorComponent*>(i->second);
			assert( s!= nullptr);
			assert(  entitysys::getByID(s->ID)->hasComponent<PlayerComponent>());

			if( entitysys::getByID(s->ID)->getComponent<PlayerComponent>()->pGUID == network::getPlayerGUID() )
			{
				s->xAng = s->xAng*Quat::RotateX( -me->mMouseMoveY/1000.0f );
				s->yAng = s->yAng*Quat::RotateY( -me->mMouseMoveX/1000.0f ); 

				// Get the physics component - set new orientation
				PhysicsComponent* phys = entitysys::getByID(s->ID)->getComponent<PhysicsComponent>();
				phys->body->setWorldTransform( btTransform( s->yAng*s->xAng , phys->body->getWorldTransform().getOrigin()));
				phys->body->activate((true));
			}

			/*
			// Dispatch event for the network
			Event* e = eventsys::get(EV_CORE_TRANSFORM_UPDATE_ORIENTATION, s->ID, this );
			TransformEvent* trans = e->createEventData<TransformEvent>();
			trans->orientation = s->yAng*s->xAng;
			eventsys::dispatch(e);
			*/
		}
		
	}
}

SpectatorManager::SpectatorManager()
{

}

void SpectatorManager::update( double dt )
{
	for( auto i = mComponents.begin(); i!=mComponents.end(); i++ )
	{

		SpectatorComponent* s = dynamic_cast<SpectatorComponent*>(i->second);
		assert(s!=nullptr);
		if( s->active )
		{
			float3 dir(0,0,0);
			float speed;

			if( s->forward )
				dir+= float3(0,0,-1);
			if( s->backward )
				dir+= float3(0,0,1);
			if( s->right )
				dir+= float3(1,0,0);
			if( s->left )
				dir+= float3(-1,0,0);
			if( s->boost )
				speed = SPECTATOR_BOOST_MULTIPLIER*SPECTATOR_DEFAULT_SPEED;
			else
				speed = SPECTATOR_DEFAULT_SPEED;

			// Apply new rotation to physics component
			PhysicsComponent* phys = entitysys::getByID(s->ID)->getComponent<PhysicsComponent>();
			phys->body->setWorldTransform( btTransform( s->yAng*s->xAng , phys->body->getWorldTransform().getOrigin()));

			// If the spectator moves, apply the force
			if( dir.Length()>0)
			{			
				phys->body->applyCentralForce( Quat(phys->body->getWorldTransform().getRotation()).Transform( dir*speed )   );		
			}
		
			// Reactivate the body
			phys->body->activate(true);
		}
	
	}
}