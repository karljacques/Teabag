#include "pch.h"
#include "networkComponent.h"

NetworkComponent::NetworkComponent( NetworkSystem* net, uint32 guid ) : 
	mNetworkSystem(net), mGUID(guid)
{
	mLastUpdate.reset();
}

void NetworkComponent::handle(Event* e)
{
	e->mGUID = mGUID;

	// TODO: Do some checking here to see if it's a sendable event 
	if( e->getEventType() == EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX || e->getEventType() == EV_CLIENT_WORLD_CREATE_STATIC_BOX )
		mNetworkSystem->send( e, PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE );
	else if ( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
	{
		if( mLastUpdate.getMilliseconds() > 500 )
		{
			mNetworkSystem->send(e, PacketPriority::LOW_PRIORITY, PacketReliability::UNRELIABLE );
			mLastUpdate.reset();
		}
	}
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
