#include "../base/component.h"

#ifndef networkComponent_h__
#define networkComponent_h__

class NetworkComponent : public Component
{
public:
	NetworkComponent()
	{
		eGUID = 0;
	}

	EntID eGUID;
};
#endif // networkComponent_h__
