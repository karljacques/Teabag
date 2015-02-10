#ifndef snapshotManager_h__
#define snapshotManager_h__

#include "engine/manager.h"

#include "engine/core/eventSystem.h"
#include "engine/core/networkSystem.h"

#include "engine/core/event/events/newSnapshotEvent.h"

#include "snapshot.h"

#define MAX_SNAPSHOTS 100

class NetworkManager;

class SnapshotManager : public Manager, public EventListener
{
public:
	SnapshotManager( NetworkManager* networkManager );
	~SnapshotManager();

	void update( double dt );
	void updateOrientation( double dt );

	void handle( Event* e ); 

	// Terminates the current snapshot, starts appending data to new snapshot
	void startNewSnapshot();

	// Send the current snapshot
	void sendSnapshot();

	// Receive and decode a snapshot
	void decodeSnapshot( unsigned char* data, unsigned int packet_size );

	// When a snapshot comes down the pipe, push it into this manager class
	void importSnapshot( Snapshot* s );

	// Get events from snapshot closest to timestamp. The latest snapshot will be taken. E.g. Snapshot at 4ms and Snapshot at 9ms, if you request 8ms snapshot you will get 9ms
	std::vector<Event*>* getSnapshotEvents( int timestamp );
	

	Ogre::Timer snapshotLife;

private:

	std::deque<Snapshot*> mSnapshots;
	NetworkManager* mNetworkManager;
	Snapshot* mCurrentSnapshot;

	Ogre::Timer lastUpdate;

};
#endif // snapshotManager_h__
