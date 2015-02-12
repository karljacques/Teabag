#ifndef snapshotManager_h__
#define snapshotManager_h__

#include "engine/manager.h"

#include "engine/component/physics/transformComponent.h"
#include "engine/component/physics/motionComponent.h"
#include "engine/component/network/networkComponent.h"

#include "engine/core/eventSystem.h"
#include "engine/core/networkSystem.h"

#include "engine/core/event/events/newSnapshotEvent.h"

#include "snapshot.h"

#include "engine/core/event/events/transfromEvent.h"
#include "engine/core/event/events/motionEvent.h"

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

	// Creates a new snapshot. Will delete an old one if the array of snapshots is full.
	Snapshot* createSnapshot();

	Ogre::Timer snapshotLife;

private:

	// Send the current snapshot
	char* _encode_snapshot( Snapshot* snapshot );

	// Convert char* to snapshot
	Snapshot* _decode_snapshot( unsigned char* data, unsigned int packet_size );

	// Calculate payload size - i.e. the size of the actual snapshot
	unsigned int _getSnapshotSize( Snapshot* snapshot );

	// Total packet size - including timestamp, number of transforms and packet ID, TIMESTAMP ID
	unsigned int _getSnapshotPacketSize( Snapshot* snapshot );

	std::deque<Snapshot*> mSnapshots;
	NetworkManager* mNetworkManager;
	Snapshot* mCurrentSnapshot;

	Ogre::Timer lastUpdate;
	bool mNewSnapshot;

};
#endif // snapshotManager_h__
