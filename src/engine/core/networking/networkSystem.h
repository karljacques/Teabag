#ifndef networkSystem_h__
#define networkSystem_h__

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include "engine/core/event/eventListener.h"

#define MAX_CONNECTIONS 16
#define SERVER_PORT 2343
#define CLIENT_PORT 2344

class NetworkSystem : public EventListener
{
public:
	NetworkSystem();

	virtual int receive() = 0;
	unsigned char getPacketIdentifier( RakNet::Packet* p );

protected:

	RakNet::RakPeerInterface * peer;
};
#endif // networkSystem_h__