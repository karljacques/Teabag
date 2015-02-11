#ifndef transfromEvent_h__
#define transfromEvent_h__

/* Macro to set position of a entity
  to be called like:
	setPosition( ID, float3(0,20,0), Quat(0,0,0,1 );*/
#define ENT_SET_TRANSFORM( id_of_entity, position_of_entity, orientation_of_entity )\
		{\
			Event* setPos_event_macro = eventsys::get(EV_CORE_SET_TRANSFORM, id_of_entity, this );\
			TransformEvent* trans_event_macro = setPos_event_macro->createEventData<TransformEvent>();\
			trans_event_macro->position = position_of_entity ;\
			trans_event_macro->orientation = orientation_of_entity;\
			eventsys::dispatchNow(setPos_event_macro );\
		}

struct TransformEvent
{
	TransformEvent()
	{
		position = float3(0,0,0);
		orientation = Quat(0,0,0,1);
	}

	float3 position;
	Quat orientation;
};
#endif // transfromEvent_h__
