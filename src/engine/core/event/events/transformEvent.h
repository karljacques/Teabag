#ifndef transformEvent_h__
#define transformEvent_h__

// Can be used to update transformation or used to apply a force.
struct TransformEvent
{
	TransformEvent()
	{
		position = float3(0,0,0);
		velocity = float3(0,0,0);
		angularVelocity = float3(0,0,0);

		orientation = Quat(0,0,0,1);
	}

	float3 position; // Used to indicate position in a transform event
	float3 velocity; // Used to dictate size, direction, velocity - when used as a transform event extension
	float3 angularVelocity; // used to specify angular velocity when used as a transform event extension

	Quat orientation; // Specifies a rotation
};

#endif // transformEvent_h__
