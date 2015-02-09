#ifndef snapshotManager_h__
#define snapshotManager_h__

#include "engine/manager.h"

#include "engine/core/eventSystem.h"
#include "engine/core/networkSystem.h"

#include "snapshot.h"

#define MAX_SNAPSHOTS 100

class NetworkManager;

class SnapshotManager : public Manager 
{
public:
	SnapshotManager( NetworkManager* networkSystem );
	~SnapshotManager();

	void update( double dt );
	void updateOrientation( double dt );

	void handle( Event* e ); /* This is not inherited from EventListener, it is not an event listener like the others, but NetworkSystem will pass on the event to it.
							Maybe it will be changed if this sort of behavior becomes common.  */

	// Terminates the current snapshot, starts appending data to new snapshot
	void startNewSnapshot();

	// Send the current snapshot
	void sendSnapshot();

	// Receive and decode a snapshot
	void decodeSnapshot( char* data, unsigned int packet_size );

	// When a snapshot comes down the pipe, push it into this manager class
	void importSnapshot( Snapshot* s );

	// Get events from snapshot closest to timestamp. The latest snapshot will be taken. E.g. Snapshot at 4ms and Snapshot at 9ms, if you request 8ms snapshot you will get 9ms
	std::vector<Event*>* getSnapshotEvents( int timestamp );

	// As the network system can be changed, this will need to be updated if the network system is changed from client to host, or vice versa
	void updateNetworkSystem( NetworkManager* networkSystem );
	

	Ogre::Timer snapshotLife;

private:

	std::deque<Snapshot*> mSnapshots;
	NetworkManager* mNetworkSystem;
	Snapshot* mCurrentSnapshot;

};
#endif // snapshotManager_h__
