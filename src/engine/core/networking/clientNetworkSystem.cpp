#include "pch.h"
#include "clientNetworkSystem.h"
#include "engine/core/graphics/UI/ogreConsole.h"

using namespace RakNet;

ClientNetworkSystem::ClientNetworkSystem( )
{
	RakNet::SocketDescriptor socketDescriptors[1] = {
		RakNet::SocketDescriptor( )
	};
	peer->Startup( 2, socketDescriptors, 1 );

	mHost = false;
}

int ClientNetworkSystem::receive()
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		OgreConsole::getSingleton().print( std::to_string( getPacketIdentifier(packet) - ID_USER_PACKET_ENUM ) );
		if( getPacketIdentifier(packet)  == ID_CONNECTION_REQUEST_ACCEPTED )
			OgreConsole::getSingleton().print("Connection Success");

		if( getPacketIdentifier(packet) == ID_CONNECTION_ATTEMPT_FAILED )
			OgreConsole::getSingleton().print("Connection Failed");

		// Is it an event? If so, we need to convert it back
		unsigned char ev = getPacketIdentifier(packet) - ID_USER_PACKET_ENUM;

		// Create data structure to copy RakNet's packet data into
		// otherwise RakNet could do some house cleaning and everything will crash.
		char* data = new char[ sizeof(Event)  ];
		memcpy( data, &packet->data[1], sizeof(Event));

		// Cast data to a transform event
		Event* te = reinterpret_cast<Event*>(data);

		EventSystem::getSingletonPtr()->dispatchEvent(te);
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

