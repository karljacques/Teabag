#include "..\base\component.h"

class SpectatorComponent : public Component
{
public:
	SpectatorComponent()
	{
		xAng = Quat(0,0,0,1);
		yAng = Quat(0,0,0,1);
	}
	float3 dir;
	float speed;

	Quat xAng;
	Quat yAng;
};