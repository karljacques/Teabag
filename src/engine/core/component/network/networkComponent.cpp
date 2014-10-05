#include "pch.h"
#include "networkComponent.h"

NetworkComponent::NetworkComponent( NetworkSystem* net, uint32 guid ) : 
	mNetworkSystem(net), mGUID(guid)
{

}

void NetworkComponent::handle(Event* e)
{
	e->mGUID = mGUID;

	// TODO: Do some checking here to see if it's a sendable event 
	mNetworkSystem->send( e );
}

NetworkComponent* NetworkSystem::createNetworkComponent()
{
	return getNetworkComponent( _find_free_guid() );
}

uint32 NetworkSystem::_find_free_guid()
{
	uint32 i = 0;
	while( mNetworkComponents[i] )
	{
		i++;
	}
	return i;
}
