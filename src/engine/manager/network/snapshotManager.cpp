#include "pch.h"
#include "snapshotManager.h"


SnapshotManager::SnapshotManager( NetworkManager* networkSystem )
{
	mNetworkManager = networkSystem;
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
		if( e->eGUID != 0 )
		{
			// Make the Transform
			TransformEvent* te = e->getData<TransformEvent>();
			Transform* trans = new Transform();
			trans->pos = te->position;
			trans->rot = te->orientation;
			trans->vel = te->velocity;
			trans->angRot = te->angularVelocity;
			trans->GUID = e->eGUID;

			bool isInSnapshot = false;
			// See if this object is already in the snapshot
			for( auto i = mCurrentSnapshot->data.begin();  i!=mCurrentSnapshot->data.end(); i++ )
			{
				if( (*i).GUID == e->eGUID )
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

	if( e->getEventType() == EV_NETWORK_INCOMING_SNAPSHOT )
	{
		// Get snapshot data
		unsigned char* start = e->getData<NewSnapshotEvent>()->start;
		unsigned int length = e->getData<NewSnapshotEvent>()->length;

		this->decodeSnapshot((char*)start,length);
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
		// Calculate payload size - i.e. the size of the actual snapshot
		unsigned int snapshot_size =  mCurrentSnapshot->data.size()*sizeof(Transform) ; //Size of transforms

		// Total packet size - including timestamp, number of transforms and packet ID, TIMESTAMP ID
		unsigned int packet_size = snapshot_size + sizeof(RakNet::Time) + sizeof(int) + 2; 

		// Create data packet
		char* payload = new char[ packet_size ];

		unsigned int offset = 0;
		payload[offset] = (unsigned char)(ID_TIMESTAMP); // First byte indicates timestamp
		offset+=1;

		// Set timestamp
		RakNet::Time time = RakNet::GetTime();
		memcpy( &payload[offset], &time, sizeof(RakNet::Time));
		offset+= sizeof(RakNet::Time);

		// Set packet ID
		payload[offset] = (unsigned char)(DPT_SNAPSHOT + ID_USER_PACKET_ENUM);
		offset+=1;
		
		// Set number of transforms
		int transforms = mCurrentSnapshot->data.size();
		memcpy( &payload[offset], &transforms, sizeof(int));
		offset+=sizeof(int);

		// Copy data to payload
		memcpy( &payload[offset], reinterpret_cast<char*>( mCurrentSnapshot->data.data() ), snapshot_size );
		offset+=snapshot_size;

		// Send the packet!!
		network::getPeer()->Send( payload,offset, IMMEDIATE_PRIORITY, RELIABLE, char(1),RakNet::UNASSIGNED_SYSTEM_ADDRESS, 1 );

		delete payload;
	}
}

void SnapshotManager::decodeSnapshot( char* data, unsigned int packet_size )
{
	// Create a snapshot we can put data into.
	Snapshot* snapshot = new Snapshot();

	// This has a timestamp, offset should be set to 1
	unsigned int offset = 1;

	// Get Timestamp
	char* ts_char = new char[ sizeof(RakNet::Time) ];
	memcpy( ts_char, &data[offset], sizeof( RakNet::Time));
	snapshot->timestamp = *reinterpret_cast<RakNet::Time*>(ts_char); // Copy into snapshot.
	offset+=sizeof(RakNet::Time);
	delete[] ts_char;

	unsigned char id = data[offset] - ID_USER_PACKET_ENUM;
	// Check packet ID
	if( id != DPT_SNAPSHOT  )
		assert( false );

	offset+=1;

	// Get number of Transforms inside the snapshot
	char* length_char = new char[sizeof(int)];
	memcpy( length_char, &data[offset], sizeof(int));
	int snapshot_length = *reinterpret_cast<int*>(length_char);
	offset+=sizeof(int);
	delete[] length_char;

	// For how many ever Transforms in the snapshot, cast the next sizeof(Transform) bytes into a snapshot, and add it to the snapshot object.
	for( int i=0; i<snapshot_length; i++ )
	{
		char* trans = new char[sizeof(Transform)];
		memcpy( trans, &data[offset], sizeof(Transform));
		snapshot->data.push_back( *reinterpret_cast<Transform*>(trans) );

		offset+=sizeof(Transform);
		delete[] trans;
	}

	// Import the new snapshot to the snapshot manager.
	this->importSnapshot(snapshot);

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
		Event* e = eventsys::get(EV_NETWORK_TRANSFORM_UPDATE);
		TransformEvent* te = e->createEventData<TransformEvent>();
		te->position = (*i).pos;
		te->orientation = (*i).rot;
		te->velocity = (*i).vel;
		te->angularVelocity = (*i).angRot;
		e->eGUID = (*i).GUID;

		vect->push_back(e);
	}

	return vect;

}

void SnapshotManager::importSnapshot(Snapshot* s)
{
	mSnapshots.push_back(s);
}

void SnapshotManager::update(double dt)
{

	if( this->snapshotLife.getMilliseconds() > 50 )
	{
		sendSnapshot();
		startNewSnapshot();
		snapshotLife.reset();
	}

	if( lastUpdate.getMilliseconds() > 50 )
	{
		if( network::getMode() == true )
		{
			this->updateOrientation(dt);
		}else
		{
			std::vector<Event*>* vect = getSnapshotEvents(0);

			for( auto i=vect->begin(); i!=vect->end(); i++ )
			{
				(*i)->ID = mNetworkManager->getIDByGUID((*i)->eGUID);
				printm("Mapped GUID " + std::to_string((*i)->eGUID) + " to ID: " + std::to_string( (*i)->ID ) );
				eventsys::dispatch(*i);
			}
			
		}
		lastUpdate.reset();
	}

}

void SnapshotManager::updateOrientation(double dt)
{
	std::vector<Event*>* vect = getSnapshotEvents(0);

	for( auto i=vect->begin(); i!=vect->end(); i++ )
	{
		(*i)->changeEventType(EV_CORE_TRANSFORM_UPDATE_ORIENTATION);
		(*i)->ID = mNetworkManager->getIDByGUID((*i)->eGUID);
		eventsys::dispatch(*i);
	}
}
