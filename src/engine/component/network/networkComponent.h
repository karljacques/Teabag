#include "engine/component/component.h"

#ifndef networkComponent_h__
#define networkComponent_h__

class NetworkComponent : public Component
{
public:
	NetworkComponent()
	{
		eGUID = 0;
	}
	
	Component* clone( EntID ID )
	{
		// Network component generally needs to be added post-creation
		assert( "This shouldn't happen" );
		NetworkComponent* comp = new NetworkComponent();
		return comp;
	}

	void activate()
	{

	}

	void deactivate()
	{

	}

	EntID eGUID;
};
#endif // networkComponent_h__
