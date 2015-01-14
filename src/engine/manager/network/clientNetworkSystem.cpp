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
			unsigned int packetSize = packet->length;

			char* data = new char[ packetSize -1];
			memcpy( data, &packet->data[1], packetSize - 1 );

			Snapshot* snapshot = new Snapshot();

			// Copy in timestamp
			char* timestamp = new char[sizeof(RakNet::Time)];
			memcpy( timestamp, &data[0], sizeof(RakNet::Time ));
			snapshot->timestamp = *reinterpret_cast<RakNet::Time*>(timestamp);
			delete timestamp;
			if( packetSize > 100 )
			{
				int hello =0;
				hello++;

			}
			for( int i=sizeof(RakNet::Time); i < packetSize -1; i+=sizeof(Transform)) 
			{
				char* trans = new char[ sizeof(Transform) ];
				memcpy( trans, &data[i], sizeof(Transform) );
				snapshot->data.push_back( *reinterpret_cast<Transform*>(trans) );
			}

			mSnapshotManager->importSnapshot(snapshot);
			std::vector<Event*>* vect = mSnapshotManager->getSnapshotEvents(0);

			for( auto i=vect->begin(); i!= vect->end(); i++ )
			{
				for( auto j=mComponents.begin(); j!=mComponents.end(); j++ )
				{
					if( j->second->GUID == (*i)->GUID )
					{
						(*i)->ID = j->second->ID;
						break;
					}					
				}

				EventSystem::getSingletonPtr()->dispatchEvent(*i);
			}
			
			
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

