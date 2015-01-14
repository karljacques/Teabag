#include "pch.h"
#include "snapshotManager.h"
#include "../event/eventSystem.h"

SnapshotManager::SnapshotManager( NetworkSystem* networkSystem )
{
	mNetworkSystem = networkSystem;
	mCurrentSnapshot = new Snapshot();
	mSnapshots.push_back(mCurrentSnapshot);

	snapshotLife.reset();
}

void SnapshotManager::handle(Event* e)
{
	// Check the event is of type TransformEvent
	// Turn the TransformEvent into a Transform
	// Put it on the snapshot - but first find if we already have this GUID there
	if( e->getEventType() == EV_CORE_TRANSFORM_UPDATE )
	{
		// Make the Transform
		TransformEvent* te = e->getData<TransformEvent>();
		Transform* trans = new Transform();
		trans->pos = te->position;
		trans->rot = te->orientation;
		trans->vel = te->velocity;
		trans->angRot = te->angularVelocity;
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

}

void SnapshotManager::sendSnapshot()
{
	if( mCurrentSnapshot->data.size() > 0 )
	{
		// Calculate payload size
		unsigned int size =  mCurrentSnapshot->data.size()*sizeof(Transform) ; //Size of transforms

		// Create data packet
		char* payload = new char[ size + sizeof(RakNet::Time) + sizeof(int) +1 ]; // +1 for ID, +1 RakNetTime for timestamp, and +1 int # of transforms

		// Set packet ID
		payload[0] = (unsigned char)(EV_NETWORK_NONEVENT_SNAPSHOT + ID_USER_PACKET_ENUM);
		unsigned int offset = 1;

		// Set timestamp
		RakNet::Time time = RakNet::GetTime();
		memcpy( &payload[offset], &time, sizeof(RakNet::Time));
		offset+= sizeof(RakNet::Time);
		
		// Set number of transforms
		int transforms = mCurrentSnapshot->data.size();
		memcpy( &payload[offset], &transforms, sizeof(int));
		offset+=sizeof(int);

		// Copy data to payload
		memcpy( &payload[offset], reinterpret_cast<char*>( mCurrentSnapshot->data.data() ), size );
		offset+=size;

		// Send the packet!!
		mNetworkSystem->getPeer()->Send( payload,offset, IMMEDIATE_PRIORITY, RELIABLE, char(1),RakNet::UNASSIGNED_SYSTEM_ADDRESS, 1 );

		delete payload;
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
		te->position = (*i).pos;
		te->orientation = (*i).rot;
		te->velocity = (*i).vel;
		te->angularVelocity = (*i).angRot;
		e->GUID = (*i).GUID;
		vect->push_back(e);
	}

	return vect;

}

void SnapshotManager::importSnapshot(Snapshot* s)
{
	mSnapshots.push_back(s);
}
