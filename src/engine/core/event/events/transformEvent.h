#ifndef transformEvent_h__
#define transformEvent_h__

#include "../event.h"

// Can be used to update transformation or used to apply a force.
struct TransformEvent
{
	TransformEvent()
	{
		mFloat3_1 = float3(0,0,0);
		mFloat3_2 = float3(0,0,0);
		mFloat3_3 = float3(0,0,0);
	}

	float3 mFloat3_1; // Used to indicate position in a transform event
	float3 mFloat3_2; // Used to dictate size, direction, velocity - when used as a transform event extension
	float3 mFloat3_3; // used to specify angular velocity when used as a transform event extension

	Quat mQuaternion; // Specifies a rotation
	Quat mQuaternion2; // Secondary rotation
};

#endif // transformEvent_h__
