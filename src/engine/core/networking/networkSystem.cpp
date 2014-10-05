#include "pch.h"
#include "networkSystem.h"

using namespace RakNet;

NetworkSystem::NetworkSystem( EventSystem* eventSystem )
	: mEventSystem( eventSystem )
{
	peer = RakNet::RakPeerInterface::GetInstance();
}

void NetworkSystem::send( Event* e )
{
	BitStream bs;

	// Writes a 'header' of sorts, with the message ID and the GUID
	bs.WriteCasted<MessageID>(e->getEventType() + ID_USER_PACKET_ENUM );
	bs.WriteCasted<unsigned int>(e->mGUID);

	// Write to BS here, dependent on the event type. Lots of casting and writing.
	// Maybe split into a separate function?

	// Send to all connected clients. Priority will need to be set based on event type
	peer->Send( &bs, PacketPriority::IMMEDIATE_PRIORITY, RELIABLE, char(1), RakNet::UNASSIGNED_SYSTEM_ADDRESS,true );
}

unsigned char NetworkSystem::getPacketIdentifier(Packet *p)
{
	if ((unsigned char)p->data[0] == ID_TIMESTAMP )

		// Returns byte after message ID and timestamp
		return (unsigned char) p->data[sizeof(MessageID) + sizeof(RakNet::Time)];
	else
		return (unsigned char) p->data[0];
}

NetworkSystem::~NetworkSystem()
{
	RakNet::RakPeerInterface::DestroyInstance( peer );
};

NetworkComponent* NetworkSystem::getNetworkComponent(uint32 guid)
{
	// If it exists, return it; if it doesn't exist, create it.
	if( mNetworkComponents[guid] )
		return mNetworkComponents[guid];
	else
	{
		// Create component and insert it into the map
		// It can be retrieved with getNetworkComponent( guid )
		NetworkComponent* net = new NetworkComponent( this, guid );
		mNetworkComponents[guid] = net;
		return net;
	}
}

void NetworkSystem::removeNetworkComponent(uint32 guid)
{
	mNetworkComponents.erase(guid);
}


bool NetworkSystem::isHost()
{
	return mHost;
}
