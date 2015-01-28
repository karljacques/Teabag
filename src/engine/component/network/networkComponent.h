#include "../base/component.h"

#ifndef networkComponent_h__
#define networkComponent_h__

class NetworkComponent : public Component
{
public:
	NetworkComponent()
	{
		GUID = 0;
	}

	EntID GUID;
};
#endif // networkComponent_h__
