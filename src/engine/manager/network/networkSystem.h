#include "networkComponentManager.h"
#include "../../core/user-interface/ogreConsole.h"

void			networkInit();
void			networkDestroy();

void			networkShutdown();
void			networkSetClient();
void			networkSetServer();

bool			networkGetMode(); 

void			networkSendEvent( Event* e, PacketPriority p, PacketReliability r );
unsigned char	networkGetPacketIdentifier( RakNet::Packet* p );

void			networkConnect( const char* ip );
int				networkGetNumberOfConnections();

void			networkHandleIncomingPackets();

int				networkPingPeerIndex( int client );

PlayerGUID		networkGetPlayerGUID();

RakNet::RakPeerInterface* networkGetPeer();