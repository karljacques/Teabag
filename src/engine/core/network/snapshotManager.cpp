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
		// Make the Transform
		TransformEvent* te = e->getData<TransformEvent>();
		Transform* trans = new Transform();
		trans->pos = te->mFloat3_1;
		trans->rot = te->mQuaternion;
		trans->vel = te->mFloat3_2;
		trans->angRot = te->mQuaternion2;
		trans->GUID = e->GUID;

		bool isInSnapshot = false;
		// See if this object is already in the snapshot
		for( auto i = mCurrentSnapshot->data.begin();  i!=mCurrentSnapshot->data.end(); i++ )
		{
			if( (*i).GUID == e->GUID )
			{
				/// It's already in the snapshot, update it with new data
				(*i) = *trans; // This should be a copy?
				isInSnapshot = true;
				break;
			}
		}

		// Wasn't already in snapshot, add it
		if( isInSnapshot == false )
		{
			mCurrentSnapshot->data.push_back(*trans); // < - This is the most likely thing to crash, if you're searching! I'm not sure if it will copy or what. For future reference.
		}

		// Cleanup
		delete trans;
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
	if( mCurrentSnapshot->data.size() > 0 )
	{
		// Calculate payload size
		unsigned int size =  mCurrentSnapshot->data.size()*sizeof(Transform) ; //Size of transforms

		// Create data packet
		char* payload = new char[ size + sizeof(RakNet::Time) +1 ]; // +1 for ID, +1 int for timestamp

		// Set packet ID
		payload[0] = (unsigned char)(EV_NETWORK_NONEVENT_SNAPSHOT + ID_USER_PACKET_ENUM);
		unsigned int offset = 1;

		// Set timestamp
		RakNet::Time time = RakNet::GetTime();
		memcpy( &payload[offset], &time, sizeof(RakNet::Time));
		offset+= sizeof(RakNet::Time);
		
		// Copy data to payload
		memcpy( &payload[offset], reinterpret_cast<char*>( mCurrentSnapshot->data.data() ), size );
		offset+=size;

		// Send the packet!!
		mNetworkSystem->getPeer()->Send( payload,size, IMMEDIATE_PRIORITY, RELIABLE, char(1),RakNet::UNASSIGNED_SYSTEM_ADDRESS, 1 );
	}
}

void SnapshotManager::updateNetworkSystem(NetworkSystem* networkSystem)
{
	mNetworkSystem = networkSystem;
}

std::vector<Event*>* SnapshotManager::getSnapshotEvents(int timestamp)
{
	// Create the vector
	std::vector<Event*>* vect = new std::vector<Event*>;

	// For the moment, just retrieve the latest snapshot
	Snapshot* snapshot =  mSnapshots.back();

	// Dispatch network transform updates
	for( auto i=snapshot->data.begin(); i!=snapshot->data.end(); i++ )
	{
		Event* e = EventSystem::getSingletonPtr()->getEvent(EV_NETWORK_TRANSFORM_UPDATE);
		TransformEvent* te = e->createEventData<TransformEvent>();
		te->mFloat3_1 = (*i).pos;
		te->mQuaternion = Quat(0,0,0,1);//(*i).rot;
		te->mFloat3_2 = (*i).vel;
		te->mQuaternion2 =Quat(0,0,0,1);//( *i).angRot;
		e->GUID = (*i).GUID;
		vect->push_back(e);
	}

	return vect;

}

void SnapshotManager::importSnapshot(Snapshot* s)
{
	mSnapshots.push_back(s);
}
