#include "pch.h"
#include "snapshotManager.h"
#include "..\event\eventSystem.h"

SnapshotManager::SnapshotManager( NetworkSystem* networkSystem )
{
	mNetworkSystem = networkSystem;
	mCurrentSnapshot = new Snapshot();
	mSnapshots.push_back(mCurrentSnapshot);
	mSnapshotIndex = 0;

	snapshotLife.reset();
}

void SnapshotManager::handle(Event* e)
{
	// Check the event is of type TransformEvent
	// Turn the TransformEvent into a Transform
	// Put it on the snapshot - but first find if we already have this GUID there
	if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
	{
		int index = mSnapshotIndex;

		// See if this object is already in the snapshot
		for( int i=0; i<SNAPSHOT_SIZE;i++)
		{
			if( mCurrentSnapshot->data[i].GUID == e->mGUID )
			{
				index = i;
				break;
			}
		}

		if( index < SNAPSHOT_SIZE )
		{
			TransformEvent* te = e->getData<TransformEvent>();
			Transform* trans = &mCurrentSnapshot->data[index];
			trans->pos = te->mFloat3_1;
			trans->rot = te->mQuaternion;
			trans->vel = te->mFloat3_2;
			trans->angRot = te->mQuaternion2;
			trans->GUID = e->ID;

		}
		// Increase snapshot index
		if( index == mSnapshotIndex )
			mSnapshotIndex++;
	}
}

SnapshotManager::~SnapshotManager()
{
	for( auto i = mSnapshots.begin(); i!=mSnapshots.end(); i++ )
	{
		delete *i;
	}
	mSnapshots.clear();
}

void SnapshotManager::startNewSnapshot()
{
	// Create a new snapshot
	mCurrentSnapshot = new Snapshot();
	mSnapshots.push_back(mCurrentSnapshot);

	// Remove oldest snapshot if we've exceeded the maximum
	if( mSnapshots.size() > MAX_SNAPSHOTS )
	{
		delete mSnapshots.front();
		mSnapshots.pop_front();
	}
	// reset index
	mSnapshotIndex = 0;
}

void SnapshotManager::sendSnapshot()
{
	// Make it into an event we can send
	// Assign timestamp and max index
	mCurrentSnapshot->timestamp = RakNet::GetTime();
	mCurrentSnapshot->maxindex = mSnapshotIndex;

	// Calculate payload size
	unsigned int size =  sizeof(Snapshot); //+1 for timestamp, +1 for index

	// Cast the snapshot to char*
	char* payload = new char[ size + 1 ]; // +1 for ID,
	memcpy( &payload[1], reinterpret_cast<char*>(mCurrentSnapshot), size );

	// Set packet ID
	payload[0] = (unsigned char)(EV_NETWORK_NONEVENT_SNAPSHOT + ID_USER_PACKET_ENUM);

	// Send the packet!!
	mNetworkSystem->getPeer()->Send( payload,size, IMMEDIATE_PRIORITY, RELIABLE, char(1),RakNet::UNASSIGNED_SYSTEM_ADDRESS, 1 );
}

void SnapshotManager::updateNetworkSystem(NetworkSystem* networkSystem)
{
	mNetworkSystem = networkSystem;
}

void SnapshotManager::getSnapshotEvents(int timestamp)
{
	// For the moment, just retrieve the latest snapshot
	Snapshot* snapshot =  mSnapshots.back();

	// Dispatch network transform updates
	for( int i=0; i<snapshot->maxindex; i++ )
	{
		Event* e = EventSystem::getSingletonPtr()->getEvent(EV_NETWORK_TRANSFORM_UPDATE);
		TransformEvent* te = e->createEventData<TransformEvent>();
		te->mFloat3_1 = snapshot->data[i].pos;
		te->mQuaternion = snapshot->data[i].rot;
		te->mFloat3_2 = snapshot->data[i].vel;
		te->mQuaternion2 = snapshot->data[i].angRot;
		e->ID = snapshot->data[i].GUID;
		EventSystem::getSingletonPtr()->dispatchEvent(e);
	}

}

void SnapshotManager::importSnapshot(Snapshot* s)
{
	mSnapshots.push_back(s);
}
