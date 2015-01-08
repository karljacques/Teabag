#include "pch.h"
#include "serverNetworkSystem.h"


using namespace RakNet;

ServerNetworkSystem::ServerNetworkSystem(  ) : NetworkSystem()
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

	if( mSnapshotManager->snapshotLife > 50 )
	{
		mSnapshotManager->sendSnapshot();
		mSnapshotManager->startNewSnapshot();
		mSnapshotManager->snapshotLife.reset();
	}

	return true;
}

void ServerNetworkSystem::handle( Event* e )
{
	if( e->getEventType() == EV_CORE_CHAT_MESSAGE || e->getEventType() == EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX )
	{
		this->send( e, IMMEDIATE_PRIORITY, RELIABLE );
	}

	if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
	{
		mSnapshotManager->handle(e);
	}
}

