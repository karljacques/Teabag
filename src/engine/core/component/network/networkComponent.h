#include "../base/component.h"
#include "engine/core/networking/NetworkSystem.h"

class NetworkComponent : 
	public Component, public EventListener
{
public:
	NetworkComponent( NetworkSystem* net );
private:
};
