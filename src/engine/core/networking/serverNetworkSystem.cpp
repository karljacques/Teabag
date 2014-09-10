#include "pch.h"
#include "serverNetworkSystem.h"

ServerNetworkSystem::ServerNetworkSystem() : NetworkSystem()
{
	// Initialise server
	peer->Startup( MAX_CONNECTIONS, &RakNet::SocketDescriptor( SERVER_PORT, 0 ), 1 );
	peer->SetMaximumIncomingConnections( MAX_CONNECTIONS );
}

int ServerNetworkSystem::receive()
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{

	} 

	return true;
}