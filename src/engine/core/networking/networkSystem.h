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
	~NetworkSystem();

	// Sends an event across the network, must be given guid of component
	void send( Event* e, RakNet::RakNetGUID guid );

	//receive packets, receive is implemented separately due to it
	// being specific to who is the authority. 
	virtual int receive() = 0;

	// Takes a packet and gives you its identifier, handles the fact
	// that it may have a timestamp
	unsigned char getPacketIdentifier( RakNet::Packet* p );

protected:

	RakNet::RakPeerInterface * peer;
};
#endif // networkSystem_h__