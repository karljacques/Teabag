#ifndef networkSystem_h__
#define networkSystem_h__

#include "../../core/event/eventListener.h"
#include "../../core/network/snapshotManager.h"
#include "../../core/entity-component/componentManager.h"
#include "../../component/network/networkComponent.h"
#include "../manager.h"

#define MAX_CONNECTIONS 16
#define SERVER_PORT 2343
#define CLIENT_PORT 2344

typedef unsigned int uint32;

enum DataPacketType
{
	DPT_Event,
	DPT_Snapshot,
	DPT_PlayerInfo
};

class NetworkSystem : public EventListener, public ComponentManager<NetworkComponent>, public Manager
{
public:
	NetworkSystem(  );
	virtual ~NetworkSystem();

	// Send and receives an event across the network
	void send( Event* e, PacketPriority p, PacketReliability r );
	void receive( Event* e );

	// Takes a packet and gives you its identifier, handles the fact
	// that it may have a timestamp
	unsigned char getPacketIdentifier( RakNet::Packet* p );

	bool isHost();
	int getConnectedClients();
	
	int pingPeer( int client );
	EntID getIDByGUID( EntID GUID );

	uint32 _find_free_guid();

	RakNet::RakPeerInterface* getPeer() {return peer;};

protected:

	char* _encode_event( Event* e, int &offset );
	Event* _decode_event( char* data );
	
	RakNet::RakPeerInterface * peer;
	bool mHost;

	EntID mGuidCount;

	SnapshotManager* mSnapshotManager;


};
#endif // networkSystem_h__