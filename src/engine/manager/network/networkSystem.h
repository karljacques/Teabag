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
	DPT_EVENT,
	DPT_SNAPSHOT
};

class NetworkSystem : public EventListener, public ComponentManager<NetworkComponent>, public Manager
{
public:
	NetworkSystem(  );
	~NetworkSystem();

	// Set client and set host
	void setAsServer();
	void setAsClient();

	// Send and receives an event across the network
	void send( Event* e, PacketPriority p, PacketReliability r );
	void receive( Event* e );

	// Takes a packet and gives you its identifier, handles the fact
	// that it may have a timestamp
	unsigned char getPacketIdentifier( RakNet::Packet* p );

	void update( double dt );

	void handle( Event* e );
	bool isHost();

	void connect( const char* ip );

	int getConnectedClients();
	
	int pingPeer( int client );
	EntID getIDByGUID( EntID GUID );

	uint32 _find_free_guid();

	RakNet::RakPeerInterface* getPeer() {return peer;};

protected:

	void _update_host( double dt );
	void _update_client( double dt );

	void _handle_host( Event* e );
	void _handle_client( Event* e );

	char* _encode_event( Event* e, int &offset );
	Event* _decode_event( char* data );
	
	RakNet::RakPeerInterface * peer;
	bool mHost;

	EntID mGuidCount;

	SnapshotManager* mSnapshotManager;


};
#endif // networkSystem_h__