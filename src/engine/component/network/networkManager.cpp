#include "pch.h"
#include "networkManager.h"

NetworkManager::NetworkManager( )
{
	mSnapshotManager = new SnapshotManager( this );
	mGuidCount = 0;
}

NetworkManager::~NetworkManager()
{
	delete mSnapshotManager;
}

void NetworkManager::update(double dt)
{
	if( network::getMode() )
	{
		this->_update_host(dt);
	}else
	{
		this->_update_client(dt);
	}
}

void NetworkManager::handle(Event* e)
{
	if( network::getMode() )
	{
		this->_handle_host(e);
	}
	else
	{
		this->_handle_client(e);
	}
}

EntID NetworkManager::getIDByGUID( EntityGUID GUID )
{
	for( auto j=mComponents.begin(); j!=mComponents.end(); j++ )
	{
		if( dynamic_cast<NetworkComponent*>(j->second)->eGUID == GUID )
		{
			assert( typeid( *j->second ) == typeid( NetworkComponent ));
			return j->second->ID;
		}					
	}

	return 0;
}

uint32 NetworkManager::_find_free_guid()
{
	mGuidCount++;
	return mGuidCount;
}

void NetworkManager::_update_host(double dt)
{
	

	if( mSnapshotManager->snapshotLife.getMilliseconds() > 50 )
	{
		mSnapshotManager->sendSnapshot();
		mSnapshotManager->startNewSnapshot();
		mSnapshotManager->snapshotLife.reset();
	}
}

void NetworkManager::_update_client(double dt)
{
	

	// Handle only orientation updates. i.e. The player has rotated.
	if( mSnapshotManager->snapshotLife.getMilliseconds() > 50 )
	{
		mSnapshotManager->sendSnapshot();
		mSnapshotManager->startNewSnapshot();
		mSnapshotManager->snapshotLife.reset();
	}
}

void NetworkManager::_handle_host(Event* e)
{

	switch( e->getEventType() )
	{
	case EV_CORE_CHAT_MESSAGE:
	case EV_CLIENT_WORLD_CREATE_DYNAMIC_BOX:
		{
			network::sendEvent(e,IMMEDIATE_PRIORITY,RELIABLE);
			break;
		}
	case EV_CORE_TRANSFORM_UPDATE:
		// Assign GUID To event
		if( this->attach_eGUID(e) )
		{
			mSnapshotManager->handle(e);
		}
		break;
	case EV_NETWORK_MOD_CLIENT:
		{
			network::shutdown();
			network::setModeClient();
			break;
		}
	}
}

void NetworkManager::_handle_client(Event* e)
{

	switch (e->getEventType())
	{
	case EV_CORE_CHAT_MESSAGE:
		break;
	case EV_NETWORK_MOD_CONNECT:
		{

			::printm("Switched to client mode");
			// Get the IP to connect to
			MessageEvent* msg = e->getData<MessageEvent>();

			// Connect to the IP
			network::connect( msg->message.c_str() );
			break;
		}
	}
}

bool NetworkManager::attach_eGUID(Event* e)
{
	// Assign GUID To event
	if( componentExists( e->ID ) )
	{
		e->eGUID = static_cast<NetworkComponent*>(mComponents[e->ID])->eGUID;
		return true;
	}

	// fail if not network component.
	return false;
}




