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

void ServerNetworkSystem::update( double dt )
{
	RakNet::Packet *packet;
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		if( getPacketIdentifier(packet) == ID_NEW_INCOMING_CONNECTION )
			OgreConsole::getSingleton().print( "Connected to a new peer" );

		if( getPacketIdentifier(packet) == ID_CONNECTION_REQUEST )
			OgreConsole::getSingleton().print( "Connection incoming..." );
	} 

	
	if( mSnapshotManager->snapshotLife.getMilliseconds() > 50 )
	{
		mSnapshotManager->sendSnapshot();
		mSnapshotManager->startNewSnapshot();
		mSnapshotManager->snapshotLife.reset();
	}
	
}

void ServerNetworkSystem::handle( Event* e )
{
	if( e->getEventType() == EV_CORE_CHAT_MESSAGE || e->getEventType() == EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX )
	{
		this->send( e, IMMEDIATE_PRIORITY, RELIABLE );
	}

	if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
	{
		// Assign GUID To event
		// TODO duplicate event to make it const-correct.
		if( componentExists( e->ID ) )
		{
			e->GUID = mComponents[e->ID]->GUID;
			mSnapshotManager->handle(e);
		}
		
	}
}

