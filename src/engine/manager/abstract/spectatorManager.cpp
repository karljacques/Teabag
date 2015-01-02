#include "pch.h"
#include "spectatorManager.h"

void SpectatorManager::handle( Event* e )
{
	/* Transform key presses into forces */
	if( e->getEventType() == EV_CORE_KEY_PRESS )
	{
		float3 dir(0,0,0);
		float speed = 20.0f;

		// Cast to key press
		KeyboardEvent* k = e->getData<KeyboardEvent>();

		switch( k->mKeycode )
		{
		case SDL_SCANCODE_W:
			dir+=float3(0,0,-1);
			break;
		case SDL_SCANCODE_A:
			dir+=float3(-1,0,0);
			break;
		case SDL_SCANCODE_S:
			dir+=float3(0,0,1);
			break;
		case SDL_SCANCODE_D:
			dir+=float3(1,0,0);
			break;
		}

		/* Loop through spectators and apply */
		for( auto i=mComponents.begin(); i!=mComponents.end();i++ )
		{
			i->second->dir = dir;
			i->second->speed = speed;

			// Apply force
			Event* ne = EventSystem::getSingletonPtr()->getEvent(EV_CORE_APPLY_FORCE);
			ne->LUID = i->second->LUID;
			TransformEvent* te = ne->createEventData<TransformEvent>();
			te->mFloat3_1 = dir*speed;
			EventSystem::getSingletonPtr()->dispatchEvent(ne);
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

			// Get the physics component - retrieve position
			PhysicsComponent* phys = mEntityManager->getByLUID(s.LUID)->getComponent<PhysicsComponent>();
			float3 pos = phys->position;
			
			// Dispatch event updating the rotation
			Event* ne = EventSystem::getSingletonPtr()->getEvent(EV_CORE_TRANSFORM_UPDATE);
			ne->LUID = s.LUID;
			TransformEvent* t = ne->createEventData<TransformEvent>();
			t->mFloat3_1 = pos;
			t->mQuaternion = s.xAng*s.yAng;
			EventSystem::getSingletonPtr()->dispatchEvent(e);
		}
		
	}
}

SpectatorManager::SpectatorManager(EntityManager* ent)
{
	mEntityManager = ent;
}
