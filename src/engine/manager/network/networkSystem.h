#ifndef networkSystem_h__
#define networkSystem_h__

#include "..\..\core\event\eventListener.h"
#include "..\..\core\network\snapshotManager.h"
#include "..\..\core\entity-component\componentManager.h"
#include "..\..\component\network\networkComponent.h"

#define MAX_CONNECTIONS 16
#define SERVER_PORT 2343
#define CLIENT_PORT 2344

typedef unsigned int uint32;

class NetworkSystem : public EventListener, public ComponentManager<NetworkComponent>
{
public:
	NetworkSystem(  );
	virtual ~NetworkSystem();

	// Sends an event across the network
	void send( Event* e, PacketPriority p, PacketReliability r );

	//receive packets, receive is implemented separately due to it
	// being specific to who is the authority. 
	virtual int receive() = 0;

	// Takes a packet and gives you its identifier, handles the fact
	// that it may have a timestamp
	unsigned char getPacketIdentifier( RakNet::Packet* p );

	// Is the system host?
	bool isHost();

	int getConnectedClients();
	
	int pingPeer( int client );

	RakNet::RakPeerInterface* getPeer() {return peer;};
	uint32 _find_free_guid();

protected:

	RakNet::RakPeerInterface * peer;
	bool mHost;

	EntID mGuidCount;

	SnapshotManager* mSnapshotManager;
};
#endif // networkSystem_h__