#ifndef networkSystem_h__
#define networkSystem_h__

#include "engine/manager/debug/ogreConsole.h"
#include "engine/core/eventSystem.h"
#include "engine/component/network/networkManager.h"

#include "engine/core/event/events/newSnapshotEvent.h"

class NetworkManager;

enum NetworkModes
{
	NET_HOST,
	NET_CLIENT
};

namespace network
{
	void init( void );
	void destroy( void );

	void shutdown( void );
	void setModeClient( void );
	void setModeServer( void );

	NetworkModes getMode( void ); 

	void sendEvent( Event* e, PacketPriority p, PacketReliability r, RakNet::RakNetGUID destination = RakNet::UNASSIGNED_RAKNET_GUID );
	unsigned char getPacketIdentifier( RakNet::Packet* p );

	void connect( const char* ip );
	int	getNumberOfConnections( void );

	void update( void );

	int	pingPeerIndex( int client );

	PlayerGUID getPlayerGUID( void );

	RakNet::RakPeerInterface* getPeer( void );
	std::shared_ptr<NetworkManager>	getNetworkManager();
}
#endif // networkSystem_h__
