#ifndef motionEvent_h__
#define motionEvent_h__

#include "common.h"

/* Macro to set motion of a entity
  to be called like:
	ENT_SET_MOTION( ID, float3(0,20,0), float(0,0,0));*/
#define ENT_SET_MOTION( id_of_entity, velocity_of_entity, angvel_of_entity )\
		{\
			Event* setMot_event_macro = eventsys::get(EVT_PHYSICS, EV_CORE_SET_MOTION, id_of_entity, this );\
			MotionEvent* mot_event_macro = setMot_event_macro->createEventData<MotionEvent>();\
			mot_event_macro->velocity = velocity_of_entity ;\
			mot_event_macro->angularVelocity = angvel_of_entity;\
			eventsys::dispatchNow(setMot_event_macro );\
		}

struct MotionEvent
{
	MotionEvent()
	{
		velocity = float3(0,0,0);
		angularVelocity = float3(0,0,0);
	}

	float3 velocity;
	float3 angularVelocity;
};
#endif // motionEvent_h__
