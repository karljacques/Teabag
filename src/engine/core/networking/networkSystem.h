#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>

#define MAX_CONNECTIONS 16
#define SERVER_PORT 2343
#define CLIENT_PORT 2344

class NetworkSystem
{
public:
	NetworkSystem();

	virtual int recieve() = 0;

	unsigned char getPacketIdentifier( RakNet::Packet* p );
protected:
	RakNet::RakPeerInterface * peer;
};