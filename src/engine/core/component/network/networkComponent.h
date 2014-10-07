#ifndef networkComponent_h__
#define networkComponent_h__

#include "common.h"
#include "pch.h"
#include "../base/component.h"
#include "engine/core/networking/NetworkSystem.h"

class NetworkSystem;

class NetworkComponent : 
	public Component, public EventListener
{
public:
	NetworkComponent( NetworkSystem* net, unsigned int guid );

	void handle( Event* e );

private:

	unsigned int mGUID;
	NetworkSystem* mNetworkSystem;

	// Time since last correctional update
	Ogre::Timer mLastUpdate;

};
#endif // networkComponent_h__
