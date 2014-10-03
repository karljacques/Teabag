#include "pch.h"
#include "networkSystem.h"

using namespace RakNet;

NetworkSystem::NetworkSystem()
{
	peer = RakNet::RakPeerInterface::GetInstance();
}

unsigned char NetworkSystem::getPacketIdentifier(Packet *p)
{
	if ((unsigned char)p->data[0] == ID_TIMESTAMP )

		// Returns byte after message ID and timestamp
		return (unsigned char) p->data[sizeof(MessageID) + sizeof(RakNet::Time)];
	else
		return (unsigned char) p->data[0];
}

NetworkSystem::~NetworkSystem()
{
	RakNet::RakPeerInterface::DestroyInstance( peer );
};