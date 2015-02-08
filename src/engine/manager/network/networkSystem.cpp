#include "pch.h"
#include "networkSystem.h"
#include "../../core/event/eventSystem.h"

#define MAX_CONNECTIONS 16
#define SERVER_PORT 2343

static NetworkComponentManager*		netCompMgr;
static RakNet::RakPeerInterface*	peer;

static bool			host;
static PlayerGUID	localGUID;


void	networkInit()
{
	netCompMgr = new NetworkComponentManager();
	peer = RakNet::RakPeerInterface::GetInstance();

	host = true;
	localGUID = RakNet::RakNetGUID::ToUint32( peer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS) ); /* It is possible that this GUID
																															could change upon shutting down RakNet
																															and restarting it. Something to bare in mind*/
	// Set as server by default.
	networkSetServer();
}

void	networkDestroy()
{
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void	networkShutdown()
{
	peer->Shutdown(0);
}

void	networkSetClient()
{
	// Must call net_shutdown() first.
	RakNet::SocketDescriptor socketDescriptors[1] = {
		RakNet::SocketDescriptor( )
	};

	peer->Shutdown(0);
	peer->Startup( 2, socketDescriptors, 1 );
	peer->SetMaximumIncomingConnections( MAX_CONNECTIONS );

	host = false;
}

void	networkSetServer()
{
	// Must call net_shutdown() first.
	RakNet::SocketDescriptor socketDescriptors[1] = {
		RakNet::SocketDescriptor( SERVER_PORT, 0 )
	};
	peer->Startup( MAX_CONNECTIONS, socketDescriptors, 1 );
	peer->SetMaximumIncomingConnections( MAX_CONNECTIONS );

	host = true;
}

bool	networkGetMode()
{
	return host;
}

char*	networkEncodeEvent(Event* e, int &offset)
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
	payload[offset] = (unsigned char)(DPT_EVENT + ID_USER_PACKET_ENUM);
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

Event*	networkDecodeEvent( char* data )
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

	if( id!= DPT_EVENT )
		return nullptr;

	offset+=2; // Add the packet ID, and we can safely skip the event type as it's duplicated inside the event itself.

	// Create an event - remember this must be deleted and cannot be inserted to the event system. Use the clone method on an
	// event that was retrieved from the event system
	char* payload = new char[ sizeof(Event) ];
	memcpy( payload, &data[offset], sizeof(Event));
	Event* e = reinterpret_cast<Event*>(payload);
	offset+=sizeof(Event);

	// Remember to delete e
	return e;

}

void	networkSendEvent(Event* e, PacketPriority p, PacketReliability r)
{
	int offset;
	char* payload = networkEncodeEvent(e,offset);
	peer->Send( payload, offset, p, r, char(1), RakNet::UNASSIGNED_SYSTEM_ADDRESS, true );
	delete[] payload;
}

unsigned char networkGetPacketIdentifier(RakNet::Packet* p)
{
	if ((unsigned char)p->data[0] == ID_TIMESTAMP )

		// Returns byte after message ID and timestamp
			return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	else
		return (unsigned char) p->data[0];
}

void	networkConnect(const char* ip)
{
	printm("Connecting to host @" + std::string(ip));
	peer->Connect( ip, SERVER_PORT, 0,0 );
}

int		networkGetNumberOfConnections()
{
	return peer->NumberOfConnections();
}

int		networkPingPeerIndex(int client)
{
	if( client < networkGetNumberOfConnections() )
	{
		// Return ping of specified client
		return peer->GetAveragePing( peer->GetSystemAddressFromIndex(client) );
	}

	return -1;
}

void	networkHandlePacketsServer()
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		unsigned int id = networkGetPacketIdentifier( packet ) - ID_USER_PACKET_ENUM;

		switch( id )
		{
		case ID_NEW_INCOMING_CONNECTION - ID_USER_PACKET_ENUM:
			printm( "Connected to a new peer" );
			break;
		case ID_CONNECTION_REQUEST - ID_USER_PACKET_ENUM:
			printm( "Connection incoming..." );
			break;
		case DPT_EVENT:
			{
				// Decode the event
				Event* tmp = networkDecodeEvent((char*)packet->data);
				assert( tmp != nullptr );
				Event* e = eventGetPooled( tmp->getEventType() );
				e->clone(tmp);
				eventDispatch(e);
				delete tmp;
				break;
			}
		case DPT_SNAPSHOT:
			// Pass packet on to the snapshot manager, which will deal with it
			//mSnapshotManager->decodeSnapshot((char*)packet->data, packet->length );
			//mSnapshotManager->updateOrientation(dt);
		default:
			printm("Packet of unhandled ID" + std::to_string( id + ID_USER_PACKET_ENUM ) );
			break;
		}
	} 
}

void	networkHandlePacketsClient()
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		// Get type of packet
		int id = (int)networkGetPacketIdentifier(packet) - (int)ID_USER_PACKET_ENUM;

		switch(id)
		{
			// RAKNET PACKETS
		case (ID_CONNECTION_REQUEST_ACCEPTED - ID_USER_PACKET_ENUM):
			{
				printm("Connection Success");

				// Inform engine that a connection has been established - carries no data
				Event* e = eventGetPooled(EV_NETWORK_NEW_CONNECTION);
				eventDispatch(e);
				break;
			}

		case (ID_CONNECTION_ATTEMPT_FAILED - ID_USER_PACKET_ENUM ):
			printm("Connection to host failed");
			break;

		case( ID_CONNECTION_LOST - ID_USER_PACKET_ENUM ):
			printm("Connection to host lost");
			break;

			// USER PACKETS (APPLICATION - not RakNet packets)
		case DPT_SNAPSHOT:
			{
				// Pass packet on to the snapshot manager, which will deal with it
				//mSnapshotManager->decodeSnapshot((char*)packet->data, packet->length );
				//mSnapshotManager->update(dt);
				break;
			}

		case DPT_EVENT:
			{
				// Decode the event
				Event* tmp = networkDecodeEvent((char*)packet->data);
				assert( tmp != nullptr );
				Event* e = eventGetPooled( tmp->getEventType() );
				e->clone(tmp);
				eventDispatch(e);
				delete tmp;

				break;
			}
		default:
			printm("Packet of unhandled ID" + std::to_string( id + ID_USER_PACKET_ENUM ) );
			break;
		}
	}
}

void	networkHandleIncomingPackets()
{
	if( networkGetMode() )
	{
		networkHandlePacketsServer();
	}else
	{
		networkHandlePacketsClient();
	}
}

PlayerGUID networkGetPlayerGUID()
{
	return localGUID;
}

RakNet::RakPeerInterface* networkGetPeer()
{
	return peer;
}
