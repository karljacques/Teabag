#include "networkSystem.h"

#ifndef clientNetworkSystem_h__
#define clientNetworkSystem_h__

class ClientNetworkSystem : public NetworkSystem
{
public:
	ClientNetworkSystem( );

	int receive();
	void connect( const char* ip );
	void handle( Event* e );

	

private:

};
#endif // clientNetworkSystem_h__