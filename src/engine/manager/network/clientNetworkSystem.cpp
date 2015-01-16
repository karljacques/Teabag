#include "pch.h"
#include "clientNetworkSystem.h"
#include "../../core/network/snapshot.h"


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
		unsigned char id = getPacketIdentifier(packet) - ID_USER_PACKET_ENUM;

		switch(id)
		{
		case DPT_Snapshot:
			// Pass packet on to the snapshot manager, which will deal with it
			mSnapshotManager->decodeSnapshot(packet->data, packet->length );
			break;
		case DPT_Event:
			// Decode the event
			Event* e = this->_decode_event(packet->data);
			break;
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

