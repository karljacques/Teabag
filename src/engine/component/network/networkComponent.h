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
	
	Component* clone()
	{
		// Network component generally needs to be added post-creation
		NetworkComponent* comp = new NetworkComponent();
		return comp;
	}

	EntID eGUID;
};
#endif // networkComponent_h__
