#include "pch.h"
#include "serverNetworkSystem.h"

ServerNetworkSystem::ServerNetworkSystem() : NetworkSystem()
{
	// Initialise server
	peer->Startup( MAX_CONNECTIONS, &RakNet::SocketDescriptor( SERVER_PORT, 0 ), 1 );
}

int ServerNetworkSystem::receive()
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		if( getPacketIdentifier(packet) == ID_CONNECTION_REQUEST_ACCEPTED )
			std::cout << "Connected to client" << std::endl;
	} 

	return true;
}