#ifndef networkComponent_h__
#define networkComponent_h__

#include "common.h"
#include "../base/component.h"
#include "engine/core/networking/NetworkSystem.h"

class NetworkSystem;

class NetworkComponent : 
	public Component, public EventListener
{
public:
	NetworkComponent( NetworkSystem* net, unsigned int guid );

	void handle( Event* e );

private:

	unsigned int mGUID;
	NetworkSystem* mNetworkSystem;

};
#endif // networkComponent_h__
