#include "pch.h"
#include "networkSystem.h"
#include "../../core/user-interface/ogreConsole.h"
#include <RakNetVersion.h>
#include "../../core/event/eventSystem.h"
#include "../../core/event/events/messageEvent.h"

using namespace RakNet;

NetworkSystem::NetworkSystem( )
{
	peer = RakNet::RakPeerInterface::GetInstance();
	mSnapshotManager = new SnapshotManager( this );
	mGuidCount = 0;

	// Startup as server
	RakNet::SocketDescriptor socketDescriptors[1] = {
		RakNet::SocketDescriptor( SERVER_PORT, 0 )
	};
	peer->Startup( MAX_CONNECTIONS, socketDescriptors, 1 );
	peer->SetMaximumIncomingConnections( MAX_CONNECTIONS );

	this->mHost = true;
	this->mLocalGUID = RakNet::RakNetGUID::ToUint32( peer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS) ); /* It is possible that this GUID
																															could change upon shutting down RakNet
																															and restarting it. Something to bare in mind*/
}

NetworkSystem::~NetworkSystem()
{
	delete mSnapshotManager;
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

void NetworkSystem::setAsServer()
{
	RakNet::SocketDescriptor socketDescriptors[1] = {
		RakNet::SocketDescriptor( SERVER_PORT, 0 )
	};
	peer->Shutdown(0);
	peer->Startup( MAX_CONNECTIONS, socketDescriptors, 1 );
	peer->SetMaximumIncomingConnections( MAX_CONNECTIONS );

	mHost = true;
}

void NetworkSystem::setAsClient()
{
	RakNet::SocketDescriptor socketDescriptors[1] = {
		RakNet::SocketDescriptor( )
	};

	peer->Shutdown(0);
	peer->Startup( 2, socketDescriptors, 1 );
	peer->SetMaximumIncomingConnections( MAX_CONNECTIONS );

	mHost = false;
}

void NetworkSystem::send( Event* e, PacketPriority p, PacketReliability r )
{
	int offset;
	char* payload = _encode_event(e,offset);
	peer->Send( payload, offset, p, r, char(1), RakNet::UNASSIGNED_SYSTEM_ADDRESS, true );
	delete[] payload;
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

void NetworkSystem::update(double dt)
{
	if( this->isHost() )
	{
		this->_update_host(dt);
	}else
	{
		this->_update_client(dt);
	}
}

void NetworkSystem::handle(Event* e)
{
	if( this->isHost() )
	{
		this->_handle_host(e);
	}
	else
	{
		this->_handle_client(e);
	}
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

EntID NetworkSystem::getIDByGUID( EntityGUID GUID )
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

uint32 NetworkSystem::_find_free_guid()
{
	mGuidCount++;
	return mGuidCount;
}

void NetworkSystem::_update_host(double dt)
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		unsigned int id = getPacketIdentifier( packet ) - ID_USER_PACKET_ENUM;

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
				Event* tmp = this->_decode_event((char*)packet->data);
				assert( tmp != nullptr );
				Event* e = EventSystem::getSingletonPtr()->getEvent( tmp->getEventType(), 0, this );
				e->clone(tmp);
				EventSystem::getSingletonPtr()->dispatchEvent(e);
				delete tmp;
				break;
			}
		default:
			printm("Packet of unhandled ID" + std::to_string( id + ID_USER_PACKET_ENUM ) );
			break;
		}
	} 

	if( mSnapshotManager->snapshotLife.getMilliseconds() > 50 )
	{
		mSnapshotManager->sendSnapshot();
		mSnapshotManager->startNewSnapshot();
		mSnapshotManager->snapshotLife.reset();
	}
}

void NetworkSystem::_update_client(double dt)
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		// Get type of packet
		int id = (int)getPacketIdentifier(packet) - (int)ID_USER_PACKET_ENUM;

		switch(id)
		{
		case (ID_CONNECTION_REQUEST_ACCEPTED - ID_USER_PACKET_ENUM):
			{
				printm("Connection Success");

				// Inform engine that a connection has been established - carries no data
				Event* e = EventSystem::getSingletonPtr()->getEvent(EV_NETWORK_NEW_CONNECTION, 0, this);
				EventSystem::getSingleton().dispatchEvent(e);
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
				mSnapshotManager->decodeSnapshot((char*)packet->data, packet->length );
				mSnapshotManager->update(dt);
				break;
			}

		case DPT_EVENT:
			{
				// Decode the event
				Event* tmp = this->_decode_event((char*)packet->data);
				assert( tmp != nullptr );
				Event* e = EventSystem::getSingletonPtr()->getEvent( tmp->getEventType(), 0, this );
				e->clone(tmp);
				EventSystem::getSingletonPtr()->dispatchEvent(e);
				delete tmp;

				break;
			}
		default:
			printm("Packet of unhandled ID" + std::to_string( id + ID_USER_PACKET_ENUM ) );
			break;
		}
	}
}

void NetworkSystem::_handle_host(Event* e)
{

	switch( e->getEventType() )
	{
	case EV_CORE_CHAT_MESSAGE:
	case EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX:
		{
			this->send(e,IMMEDIATE_PRIORITY,RELIABLE);
			break;
		}
	case EV_CORE_TRANSFORM_UPDATE:
		// Assign GUID To event
		if( componentExists( e->ID ) )
		{
			e->eGUID = mComponents[e->ID]->GUID;
			mSnapshotManager->handle(e);
		}
		break;
	case EV_NETWORK_MOD_CLIENT:
		{
			this->setAsClient();
			break;
		}
	}
}

void NetworkSystem::_handle_client(Event* e)
{

	switch (e->getEventType())
	{
	case EV_CORE_CHAT_MESSAGE:
		break;
	case EV_NETWORK_MOD_CONNECT:
		{
			printm("Switched to client mode");
			// Get the IP to connect to
			MessageEvent* msg = e->getData<MessageEvent>();

			// Connect to the IP
			this->connect( msg->message.c_str() );
		}
	}
}

void NetworkSystem::connect(const char* ip)
{
	printm("Connecting to host @" + std::string(ip));
	peer->Connect( ip, SERVER_PORT, 0,0 );
}

PlayerGUID NetworkSystem::getLocalGUID()
{
	return this->mLocalGUID;
}



