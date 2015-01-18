#include "pch.h"
#include "clientNetworkSystem.h"
#include "../../core/network/snapshot.h"
#include <RakNetVersion.h>

using namespace RakNet;

ClientNetworkSystem::ClientNetworkSystem( ) : NetworkSystem()
{
	RakNet::SocketDescriptor socketDescriptors[1] = {
		RakNet::SocketDescriptor( )
	};
	peer->Startup( 2, socketDescriptors, 1 );

	mHost = false;
}

void ClientNetworkSystem::update( double dt )
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		if( getPacketIdentifier(packet)  == ID_CONNECTION_REQUEST_ACCEPTED )
			OgreConsole::getSingleton().print("Connection Success");

		if( getPacketIdentifier(packet) == ID_CONNECTION_ATTEMPT_FAILED )
			OgreConsole::getSingleton().print("Connection to host failed");

		if( getPacketIdentifier(packet) == ID_CONNECTION_LOST )
			OgreConsole::getSingleton().print("Connect to host lost");

		// Get type of packet
		int id = (int)getPacketIdentifier(packet) - (int)ID_USER_PACKET_ENUM;

		switch(id)
		{
		case DPT_Snapshot:
			{
				// Pass packet on to the snapshot manager, which will deal with it
				mSnapshotManager->decodeSnapshot((char*)packet->data, packet->length );
				mSnapshotManager->update(dt);
				break;
			}
		case DPT_Event:
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
			{
				// Print the code of the packet recieved, since it matches nothing
				OgreConsole::getSingleton().print( "Error - packet ID:" + std::to_string( getPacketIdentifier(packet) ) );
				OgreConsole::getSingleton().print( "RAKNET_PROTOCOL_VERSION_REMOTE:" + std::to_string( (int)packet->data[1] ));
				OgreConsole::getSingleton().print( "RAKNET_PROTOCOL_VERSION_LOCAL:" + std::to_string( (int)( RAKNET_PROTOCOL_VERSION ) ) );
				break;
			}
		}
	}
}

void ClientNetworkSystem::connect( const char* ip )
{
	peer->Connect(ip, SERVER_PORT, 0,0 );
}

void ClientNetworkSystem::handle( Event* e )
{
	if( e->getEventType() == EV_CORE_CHAT_MESSAGE )
	{
		this->send( e, IMMEDIATE_PRIORITY, RELIABLE );
	}
}

