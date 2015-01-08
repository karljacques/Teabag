#include "pch.h"
#include "clientNetworkSystem.h"
#include "..\..\core\network\snapshot.h"


using namespace RakNet;

ClientNetworkSystem::ClientNetworkSystem( ) : NetworkSystem()
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
		if( getPacketIdentifier(packet)  == ID_CONNECTION_REQUEST_ACCEPTED )
			OgreConsole::getSingleton().print("Connection Success");

		if( getPacketIdentifier(packet) == ID_CONNECTION_ATTEMPT_FAILED )
			OgreConsole::getSingleton().print("Connection Failed");

		// Is it an event? If so, we need to convert it back
		unsigned char id = getPacketIdentifier(packet) - ID_USER_PACKET_ENUM;

		// Create data structure to copy RakNet's packet data into
		// otherwise RakNet could do some house cleaning and everything will crash.
		if( id == EV_NETWORK_NONEVENT_SNAPSHOT )
		{
			char* data = new char[sizeof(Snapshot)];
			memcpy( data, &packet->data[1], sizeof(Snapshot) );

			Snapshot* snapshot = reinterpret_cast<Snapshot*>(data);
			mSnapshotManager->importSnapshot(snapshot);
			mSnapshotManager->getSnapshotEvents(0);
		}else
		{
			char* data = new char[ sizeof(Event)  ];
			memcpy( data, &packet->data[1], sizeof(Event));

			// Cast data to a transform event
			Event* te = reinterpret_cast<Event*>(data);

			EventSystem::getSingletonPtr()->dispatchEvent(te);
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
	if( e->getEventType() == EV_CORE_CHAT_MESSAGE )
	{
		this->send( e, IMMEDIATE_PRIORITY, RELIABLE );
	}
}

