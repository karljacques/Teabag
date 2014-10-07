#include "pch.h"
#include "clientNetworkSystem.h"
#include "engine/core/graphics/UI/ogreConsole.h"

using namespace RakNet;

ClientNetworkSystem::ClientNetworkSystem( EventSystem* eventSystem )
	: NetworkSystem( eventSystem )
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

		switch( ev )
		{
		case EV_CLIENT_WORLD_CREATE_STATIC_BOX:
			{
				// Create data structure to copy RakNet's packet data into
				// otherwise RakNet could do some house cleaning and everything will crash.
				char* data = new char[ sizeof(TransformEvent)  ];
				memcpy( data, &packet->data[1], sizeof(TransformEvent));

				// Cast data to a transform event
				TransformEvent* te = reinterpret_cast<TransformEvent*>(data);

				mEventSystem->dispatchEvent(te);
			}
		break;
		case EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX:
			{
				OgreConsole::getSingleton().print( "Spawning Cube..." );

				// Create data structure to copy RakNet's packet data into
				// otherwise RakNet could do some house cleaning and everything will crash.
				char* data = new char[ sizeof(TransformEvent)  ];
				memcpy( data, &packet->data[1], sizeof(TransformEvent));

				// Cast data to a transform event
				TransformEvent* te = reinterpret_cast<TransformEvent*>(data);
				
				mEventSystem->dispatchEvent(te);
			}
		case EV_CORE_TRANSFORM_UPDATE:
			{
				// Create data structure to copy RakNet's packet data into
				// otherwise RakNet could do some house cleaning and everything will crash.
				char* data = new char[ sizeof(TransformEvent)  ];
				memcpy( data, &packet->data[1], sizeof(TransformEvent));

				// Cast data to a transform event
				TransformEvent* te = reinterpret_cast<TransformEvent*>(data);
				te->changeEventType( EV_NETWORK_TRANSFORM_UPDATE );

				if( mNetworkComponents[te->mGUID])
					mNetworkComponents[te->mGUID]->dispatch(te);
			}
	}
	break;
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

