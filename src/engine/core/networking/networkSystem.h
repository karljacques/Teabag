#ifndef networkSystem_h__
#define networkSystem_h__

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>

#include "engine/core/event/eventListener.h"
#include "engine/core/component/network/networkComponent.h"

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

	// Handles the creation and synchronization of network components
	void createNetworkComponent( RakNet::RakNetGUID guid );
	void removeNetworkComponent( RakNet::RakNetGUID guid ); // Call this whilst you still have the GUID, but this method does not delete the component or unregister it, you still need to do this!
	NetworkComponent* getNetworkComponent( RakNetGUID guid );

	// Is the system host?
	bool isHost();

protected:

	// Map of network components
	std::map<RakNet::RakNetGUID,NetworkComponent*> mNetworkComponents;

	RakNet::RakPeerInterface * peer;

	bool mHost;
};
#endif // networkSystem_h__