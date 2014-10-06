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
	// Cast event to char*, make room for the packet ID
	char* payload = new char[ e->getSize() + 1 ];

	// Copy in casted event to the payload, offset by 1 byte
	memcpy( &payload[1],reinterpret_cast<char*>(e), e->getSize() );

	// Set packet ID
	payload[0] = (unsigned char)(e->getEventType() + ID_USER_PACKET_ENUM);

	// Send packet
	peer->Send( payload, e->getSize()+1, PacketPriority::IMMEDIATE_PRIORITY, PacketReliability::RELIABLE, char(1), RakNet::UNASSIGNED_SYSTEM_ADDRESS, 1 );

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
