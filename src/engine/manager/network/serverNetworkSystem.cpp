#include "pch.h"
#include "serverNetworkSystem.h"


using namespace RakNet;

ServerNetworkSystem::ServerNetworkSystem(  )
{
	// Initialise server
	RakNet::SocketDescriptor socketDescriptors[1] = {
		RakNet::SocketDescriptor( SERVER_PORT, 0 )
	};
	peer->Startup( MAX_CONNECTIONS, socketDescriptors, 1 );

	peer->SetMaximumIncomingConnections( MAX_CONNECTIONS );

	mHost = true;
}

int ServerNetworkSystem::receive()
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		if( getPacketIdentifier(packet) == ID_NEW_INCOMING_CONNECTION )
			OgreConsole::getSingleton().print( "Connected to a new peer" );
	} 

	return true;
}

void ServerNetworkSystem::handle( Event* e )
{
	if( e->getEventType() == EV_CORE_CHAT_MESSAGE )
	{
		this->send( e, IMMEDIATE_PRIORITY, RELIABLE );
	}
}

