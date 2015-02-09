#ifndef spectatorComponent_h__
#define spectatorComponent_h__

#include "engine/component/component.h"

class SpectatorComponent : public Component
{
public:
	SpectatorComponent()
	{
		xAng = Quat(0,0,0,1);
		yAng = Quat(0,0,0,1);

		dir = float3(0,0,0);

		forward = false;
		backward = false;
		left = false;
		right = false;

		boost = false;
	}

	Component* clone()
	{
		return new SpectatorComponent();
	}

	float3 dir;
	float speed;

	Quat xAng;
	Quat yAng;

	bool forward;
	bool backward;
	bool left;
	bool right;

	bool boost;

};
#endif // spectatorComponent_h__