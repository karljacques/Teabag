#ifndef serverNetworkSystem_h__
#define serverNetworkSystem_h__

#include "pch.h"
#include "networkSystem.h"
#include "..\..\core\user-interface\ogreConsole.h"

class ServerNetworkSystem : public NetworkSystem
{
public:
	ServerNetworkSystem(  );

	int receive();
	void handle( Event* e );

private:

	
};
#endif // serverNetworkSystem_h__