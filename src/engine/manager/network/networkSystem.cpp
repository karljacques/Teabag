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
	int offset;
	char* payload = _encode_event(e,offset);
	peer->Send( payload, offset, p, r, char(1), RakNet::UNASSIGNED_SYSTEM_ADDRESS, 1 );
	delete[] payload;
}

void NetworkSystem::receive( Event* e )
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

EntID NetworkSystem::getIDByGUID( EntID GUID )
{
	for( auto j=mComponents.begin(); j!=mComponents.end(); j++ )
	{
		if( j->second->GUID == GUID )
		{
			return j->second->ID;
		}					
	}

	return 0;
}

char* NetworkSystem::_encode_event(Event* e, int &offset )
{
	// Cast event to char*, make room for the packet ID and timestamp, and TIMESTAMP_ID
	char* payload = new char[ sizeof(Event) + 11 ];

	offset = 0;

	// Inform of timestamp
	payload[0] = (unsigned char)(ID_TIMESTAMP);
	offset+=1;

	// Attach timestamp
	RakNet::Time time = RakNet::GetTime();
	memcpy( &payload[offset], &time, sizeof(RakNet::Time));
	offset+= sizeof(RakNet::Time);

	// Set packet ID
	payload[offset] = (unsigned char)(DPT_Event + ID_USER_PACKET_ENUM);
	offset+=1;

	// Set event type
	payload[offset] = (unsigned char)(e->getEventType() );
	offset+=1;

	// Copy in casted event to the payload, offset by 1 byte
	memcpy( &payload[offset],reinterpret_cast<char*>(e), sizeof(Event) );
	offset+=sizeof(Event);

	// Must delete payload
	return payload;
}

Event* NetworkSystem::_decode_event( char* data )
{
	// Where to read from - has a timestamp so first byte is just ID_TIMESTAMP
	int offset = 1;

	/////////////////////
	// Get packet headers
	/////////////////////

	//Time
	RakNet::Time time =0;
	memcpy( &time, &data[offset], sizeof(RakNet::Time) );
	offset+=sizeof(RakNet::Time);

	// Get packet ID - This needs to be DPT_Event
	unsigned char id = data[offset] - ID_USER_PACKET_ENUM;

	if( id!= DPT_Event )
		return nullptr;

	offset+=2; // Add the packet ID, and we can safely skip the event type as it's duplicated inside the event itself.

	// Create an event - remember this must be deleted and cannot be inserted to the event system. Use the clone method on an
	// event that was retrieved from the event system
	char* payload = new char[ sizeof(Event) ];
	memcpy( payload, &data[offset], sizeof(Event));
	Event* e = reinterpret_cast<Event*>(payload);

	// Remember to delete e
	return e;

}

uint32 NetworkSystem::_find_free_guid()
{
	mGuidCount++;
	return mGuidCount;
}


