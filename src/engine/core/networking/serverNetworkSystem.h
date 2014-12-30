#include "pch.h"
#include "networkSystem.h"

#ifndef serverNetworkSystem_h__
#define serverNetworkSystem_h__

class ServerNetworkSystem : public NetworkSystem
{
public:
	ServerNetworkSystem(  );

	int receive();
	void handle( Event* e );



private:

	
};
#endif // serverNetworkSystem_h__