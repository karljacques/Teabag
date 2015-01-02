#ifndef clientNetworkSystem_h__
#define clientNetworkSystem_h__

#include "networkSystem.h"
#include "..\..\core\render\UI\ogreConsole.h"

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