#ifndef networkComponentManager_h__
#define networkComponentManager_h__

#include "networkComponent.h"

#include "engine/component/componentManager.h"
#include "engine/manager/network/snapshotManager.h"
#include "engine/manager/debug/ogreConsole.h"
#include "engine/manager.h"

#include "engine/core/eventSystem.h"

#include "engine/core/event/events/messageEvent.h"

typedef unsigned int uint32;

enum DataPacketType
{
	DPT_EVENT,
	DPT_SNAPSHOT
};

class SnapshotManager;

class NetworkManager : public EventListener, public ComponentManager, public Manager
{
public:
	NetworkManager( void );
	~NetworkManager( void );

	void				update( double dt );
	void				handle( Event* e );

	EntID				getIDByGUID( EntityGUID GUID );
	EntityGUID			_find_free_guid();

	// Takes an event with an ID and assigns the GUID of the event if it has one.
	// WARNING: modifies an event whilst it is live in the event system.
	// const correctness has been blown out of the window. (not that there was any, anyway! )
	bool				attach_eGUID( Event* e );

	// Getters
	SnapshotManager*	getShapshotManager() { return mSnapshotManager; };

protected:

	void				_update_host( double dt );
	void				_update_client( double dt );

	void				_handle_host( Event* e );
	void				_handle_client( Event* e );

	EntityGUID				mGuidCount;

	SnapshotManager*	mSnapshotManager;

};
#endif // networkComponentManager_h__