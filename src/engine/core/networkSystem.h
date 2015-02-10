#ifndef networkSystem_h__
#define networkSystem_h__

#include "engine/manager/debug/ogreConsole.h"
#include "engine/core/eventSystem.h"
#include "engine/component/network/networkManager.h"

#include "engine/core/event/events/newSnapshotEvent.h"

class NetworkManager;

namespace network
{
	void init( void );
	void destroy( void );

	void shutdown( void );
	void setModeClient( void );
	void setModeServer( void );

	bool getMode( void ); 

	void sendEvent( Event* e, PacketPriority p, PacketReliability r );
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
