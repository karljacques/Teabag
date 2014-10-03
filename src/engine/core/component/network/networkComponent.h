#ifndef networkComponent_h__
#define networkComponent_h__

#include "../base/component.h"
#include "engine/core/networking/NetworkSystem.h"

class NetworkSystem;

class NetworkComponent : 
	public Component, public EventListener
{
public:
	NetworkComponent( NetworkSystem* net, RakNet::RakNetGUID guid );

	void handle( Event* e );

private:

	RakNet::RakNetGUID mGUID;
	NetworkSystem* mNetworkSystem;

};
#endif // networkComponent_h__
