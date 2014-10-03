#include "pch.h"
#include "networkComponent.h"

NetworkComponent::NetworkComponent( NetworkSystem* net, RakNet::RakNetGUID guid ) : 
	mNetworkSystem(net), mGUID(guid)
{

}

void NetworkComponent::handle(Event* e)
{

}

