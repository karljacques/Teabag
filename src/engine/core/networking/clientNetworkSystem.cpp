#include "pch.h"
#include "clientNetworkSystem.h"

using namespace RakNet;

ClientNetworkSystem::ClientNetworkSystem()
{
	const char* hostip = "192.168.0.1";

	peer->Startup( 2, &RakNet::SocketDescriptor(), 1 );
	peer->Connect( hostip, SERVER_PORT, 0,0 );
}

int ClientNetworkSystem::recieve()
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		if( getPacketIdentifier(packet) == ID_CONNECTION_REQUEST_ACCEPTED )
			std::cout << "Connected to server" << std::endl;
	} 

	return true;
}