#ifndef clientNetworkSystem_h__
#define clientNetworkSystem_h__

#include "networkSystem.h"
#include "../../core/user-interface/ogreConsole.h"

class ClientNetworkSystem : public NetworkSystem
{
public:
	ClientNetworkSystem( );

	void update( double dt );
	void connect( const char* ip );
	void handle( Event* e );

private:

};
#endif // clientNetworkSystem_h__