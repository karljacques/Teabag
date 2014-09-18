#include "networkSystem.h"

class ClientNetworkSystem : public NetworkSystem
{
public:
	ClientNetworkSystem();

	int receive();
	void connect( const char* ip );

private:

};