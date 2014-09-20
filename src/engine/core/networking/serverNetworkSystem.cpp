#include "pch.h"
#include "serverNetworkSystem.h"
#include "engine/core/graphics/UI/ogreConsole.h"

using namespace RakNet;

ServerNetworkSystem::ServerNetworkSystem() : NetworkSystem()
{
	// Initialise server
	RakNet::SocketDescriptor socketDescriptors[1] = {
		RakNet::SocketDescriptor( SERVER_PORT, 0 )
	};
	peer->Startup( MAX_CONNECTIONS, socketDescriptors, 1 );

	peer->SetMaximumIncomingConnections( MAX_CONNECTIONS );
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
	switch( e->getEventType() )
	{
	case EV_CORE_MOUSE_MOVEMENT||EV_CORE_MOUSE_PRESS||EV_CORE_MOUSE_RELEASE:
		{
			MouseEvent me = *static_cast<MouseEvent*>(e);
			RakNet::BitStream* bs = new RakNet::BitStream();
			bs->Write( (RakNet::MessageID)e->getEventType() );
			bs->Write( me );
			peer->Send( bs, PacketPriority::IMMEDIATE_PRIORITY, RELIABLE, char(1), RakNet::UNASSIGNED_SYSTEM_ADDRESS,true );
		}
		
	}
}