#include "..\event\eventListener.h"
#include "..\..\manager\network\networkSystem.h"
#include "snapshot.h"

#ifndef snapshotManager_h__
#define snapshotManager_h__

#define MAX_SNAPSHOTS 100

class SnapshotManager
{
public:
	SnapshotManager( NetworkSystem* networkSystem );
	~SnapshotManager();

	void handle( Event* e ); /* This is not inherited from EventListener, it is not an event listener like the others, but NetworkSystem will pass on the event to it.
							Maybe it will be changed if this sort of behavior becomes common.  */

	// Terminates the current snapshot, starts appending data to new snapshot
	void startNewSnapshot();

	// Send the current snapshot
	void sendSnapshot();

	// When a snapshot comes down the pipe, push it into this manager class
	void importSnapshot( Snapshot* s );

	// Get events from snapshot closest to timestamp. The latest snapshot will be taken. E.g. Snapshot at 4ms and Snapshot at 9ms, if you request 8ms snapshot you will get 9ms
	std::vector<Event*> getSnapshotEvents( int timestamp );

	// As the network system can be changed, this will need to be updated if the network system is changed from client to host, or vice versa
	void updateNetworkSystem( NetworkSystem* networkSystem );
	
	Ogre::Timer snapshotLife;

private:

	std::deque<Snapshot*> mSnapshots;
	NetworkSystem* mNetworkSystem;
	Snapshot* mCurrentSnapshot;
	int mSnapshotIndex;
	

};
#endif // snapshotManager_h__
