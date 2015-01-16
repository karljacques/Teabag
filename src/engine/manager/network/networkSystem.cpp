#include "pch.h"
#include "networkSystem.h"

using namespace RakNet;

NetworkSystem::NetworkSystem( )
{
	peer = RakNet::RakPeerInterface::GetInstance();
	mSnapshotManager = new SnapshotManager( this );
	mGuidCount = 0;
}

NetworkSystem::~NetworkSystem()
{
	delete mSnapshotManager;
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void NetworkSystem::send( Event* e, PacketPriority p, PacketReliability r )
{
	// Cast event to char*, make room for the packet ID
	char* payload = new char[ sizeof(Event) + 1 ];

	// Copy in casted event to the payload, offset by 1 byte
	memcpy( &payload[1],reinterpret_cast<char*>(e), sizeof(Event) );

	// Set packet ID
	payload[0] = (unsigned char)(e->getEventType() + ID_USER_PACKET_ENUM);

	// Send packet
	peer->Send( payload, sizeof(Event)+1, p, r, char(1), RakNet::UNASSIGNED_SYSTEM_ADDRESS, 1 );

}

void NetworkSystem::send(char* d, DataPacketType type, PacketPriority p, PacketReliability r)
{

}

unsigned char NetworkSystem::getPacketIdentifier(Packet *p)
{
	if ((unsigned char)p->data[0] == ID_TIMESTAMP )

		// Returns byte after message ID and timestamp
		return (unsigned char) p->data[sizeof(MessageID) + sizeof(RakNet::Time)];
	else
		return (unsigned char) p->data[0];
}


bool NetworkSystem::isHost()
{
	return mHost;
}

int NetworkSystem::getConnectedClients()
{
	return peer->NumberOfConnections();
}

int NetworkSystem::pingPeer(int client)
{
	if( client < this->getConnectedClients() )
	{
		// Return ping of specified client
		return peer->GetAveragePing( peer->GetSystemAddressFromIndex(client) );
	}

	return -1;
}

uint32 NetworkSystem::_find_free_guid()
{
	mGuidCount++;
	return mGuidCount;
}
