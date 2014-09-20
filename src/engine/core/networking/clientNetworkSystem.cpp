#include "pch.h"
#include "clientNetworkSystem.h"
#include "engine/core/graphics/UI/ogreConsole.h"

using namespace RakNet;

ClientNetworkSystem::ClientNetworkSystem()
{
	RakNet::SocketDescriptor socketDescriptors[1] = {
		RakNet::SocketDescriptor()
	};
	peer->Startup( 2, socketDescriptors, 1 );
}

int ClientNetworkSystem::receive()
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		if( getPacketIdentifier(packet) == ID_CONNECTION_REQUEST_ACCEPTED )
		{
			OgreConsole::getSingleton().print("Connection Success");
			return ID_CONNECTION_REQUEST_ACCEPTED;
		}

		if( getPacketIdentifier(packet) == EV_CORE_MOUSE_MOVEMENT )
		{
			OgreConsole::getSingleton().print("Received a mouse move");
		}
	} 

	return true;
}

void ClientNetworkSystem::connect( const char* ip )
{
	peer->Connect(ip, SERVER_PORT, 0,0 );
}

void ClientNetworkSystem::handle( Event* e )
{

}