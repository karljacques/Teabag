#ifndef networkSystem_h__
#define networkSystem_h__

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>

#define MAX_CONNECTIONS 16
#define SERVER_PORT 2343
#define CLIENT_PORT 2344

class NetworkSystem
{
public:
	NetworkSystem();

	virtual int receive() = 0;

	unsigned char getPacketIdentifier( RakNet::Packet* p );
protected:
	RakNet::RakPeerInterface * peer;
};
#endif // networkSystem_h__