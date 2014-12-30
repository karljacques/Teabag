#ifndef networkSystem_h__
#define networkSystem_h__

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>

#include "engine/core/event/eventSystem.h"
#include "engine/core/event/eventListener.h"


#define MAX_CONNECTIONS 16
#define SERVER_PORT 2343
#define CLIENT_PORT 2344

typedef unsigned int uint32;

class NetworkComponent;

class NetworkSystem : public EventListener
{
public:
	NetworkSystem(  );
	~NetworkSystem();

	// Sends an event across the network
	void send( Event* e, PacketPriority p, PacketReliability r );

	//receive packets, receive is implemented separately due to it
	// being specific to who is the authority. 
	virtual int receive() = 0;

	// Takes a packet and gives you its identifier, handles the fact
	// that it may have a timestamp
	unsigned char getPacketIdentifier( RakNet::Packet* p );

	// Handles the creation and synchronization of network components
	NetworkComponent* createNetworkComponent();
	void removeNetworkComponent( uint32 guid ); // Call this whilst you still have the GUID, but this method does not delete the component or unregister it, you still need to do this!
	NetworkComponent* getNetworkComponent( uint32 guid );

	// Is the system host?
	bool isHost();

	int getConnectedClients();
	
	int pingPeer( int client );

protected:

	// Map of network components
	std::map<uint32,NetworkComponent*> mNetworkComponents;
	RakNet::RakPeerInterface * peer;
	bool mHost;

	uint32 _find_free_guid();
};
#endif // networkSystem_h__