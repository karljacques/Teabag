#include "pch.h"
#include "snapshotManager.h"

#include "engine/manager/debug/ogreConsole.h"

SnapshotManager::SnapshotManager( NetworkManager* networkSystem )
{
	mNetworkManager = networkSystem;
	mCurrentSnapshot = createSnapshot();

	snapshotLife.reset();
	mNewSnapshot = false;
}

void SnapshotManager::handle(Event* e)
{
	if( e->getEventID() == EV_NETWORK_INCOMING_SNAPSHOT )
	{
		// Get snapshot data
		unsigned char* start = e->getData<NewSnapshotEvent>()->start;
		unsigned int length = e->getData<NewSnapshotEvent>()->length;

		Snapshot* incoming = this->_decode_snapshot(start,length);
		mSnapshots.push_back(incoming);
		mNewSnapshot = true;
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

Snapshot* SnapshotManager::createSnapshot()
{
	// Create a new snapshot
	Snapshot* snapshot = new Snapshot();
	mSnapshots.push_back(snapshot);

	// Remove oldest snapshot if we've exceeded the maximum
	if( mSnapshots.size() > MAX_SNAPSHOTS )
	{
		delete mSnapshots.front();
		mSnapshots.pop_front();
	}

	return snapshot;

}

void SnapshotManager::update(double dt)
{

	//Differentiate between behavior on the host and behavior on the client.
	if( network::getMode() == NET_HOST )
	{
		// RUNNING ON HOST
		// Loop through all networked components, get their parent entities and update the snapshot based
		// on their TransformComponent and MotionComponent
		// SnapshotManager is friend to the NetworkManager so we can access it's components.
		for( auto i=mNetworkManager->mComponents.begin(); i!=mNetworkManager->mComponents.end(); i++ )
		{
			// Get Entity
			Entity* ent = entitysys::getByID(i->first);

			// Get Position Component
			if( ent->hasComponent<TransformComponent>())
			{
				TransformComponent* trans_comp = ent->getComponent<TransformComponent>();

				// Create a Transform
				Transform* trans = new Transform();
				trans->GUID = static_cast<NetworkComponent*>(i->second)->eGUID;
				trans->pos = trans_comp->position;
				trans->rot = trans_comp->orientation;

				// Get MotionComponent
				if( ent->hasComponent<MotionComponent>() )
				{
					MotionComponent* motion = ent->getComponent<MotionComponent>();

					// Append motion data
					trans->vel = motion->velocity;
					trans->angRot = motion->angularVelocity;
				}

				// Add Transform to snapshot
				this->mCurrentSnapshot->addTransform(trans);
				delete trans;
			}
		}
		// Host mode
		if( this->snapshotLife.getMilliseconds() > 50 )
		{

			// If there are snapshots to send, do so.
			if( mCurrentSnapshot->data.size() > 0 )
			{
				// Convert current snapshot to char*
				char* payload = _encode_snapshot(mCurrentSnapshot);
				unsigned int offset = _getSnapshotPacketSize(mCurrentSnapshot);

				assert( payload != nullptr );
				// Send snapshot packet
				network::getPeer()->Send( payload,offset, IMMEDIATE_PRIORITY, RELIABLE, char(1),RakNet::UNASSIGNED_SYSTEM_ADDRESS, 1 );
				delete payload;
			}

			// Create a new snapshot
			mCurrentSnapshot = createSnapshot();
			this->snapshotLife.reset();
		}

	}
	else
	{
		if( mNewSnapshot )
		{
			// RUNNING ON CLIENT
			// Get the lastest snapshot
			Snapshot* snapshot =  mSnapshots.back();
			mNewSnapshot = false;

			// Dispatch network transform updates
			for( auto i=snapshot->data.begin(); i!=snapshot->data.end(); i++ )
			{
				EntID ID = mNetworkManager->getIDByGUID(i->GUID);

				// Check for existence of entity
				if( entitysys::entityExists( ID ) )
				{
					Entity* ent = entitysys::getByID(ID);

					// Dispatch events to update position
					ENT_SET_TRANSFORM( ID, i->pos, i->rot );
					ENT_SET_MOTION( ID, i->vel, i->angRot );
				}
			}
		}

	}


}

void SnapshotManager::updateOrientation(double dt)
{

}

char* SnapshotManager::_encode_snapshot( Snapshot* snapshot )
{
	if( snapshot->data.size() > 0 )
	{	
		// Calculate payload size - i.e. the size of the actual snapshot
		unsigned int snapshot_size = _getSnapshotSize(snapshot);

		// Total packet size - including timestamp, number of transforms and packet ID, TIMESTAMP ID
		unsigned int packet_size = _getSnapshotPacketSize(snapshot);

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
		int transforms = snapshot->data.size();
		memcpy( &payload[offset], &transforms, sizeof(int));
		offset+=sizeof(int);

		// Copy data to payload
		memcpy( &payload[offset], reinterpret_cast<char*>( snapshot->data.data() ), snapshot_size );
		offset+=snapshot_size;

		return payload;
	}

	// Snapshot empty, return null.
	return nullptr;
}

Snapshot* SnapshotManager::_decode_snapshot( unsigned char* data, unsigned int packet_size )
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

	return snapshot;

}

unsigned int SnapshotManager::_getSnapshotSize(Snapshot* snapshot)
{
	// Calculate payload size - i.e. the size of the actual snapshot
	return snapshot->data.size()*sizeof(Transform) ; //Size of transforms
}

unsigned int SnapshotManager::_getSnapshotPacketSize(Snapshot* snapshot)
{
	// Total packet size - including timestamp, number of transforms and packet ID, TIMESTAMP ID
	return _getSnapshotSize( snapshot ) + sizeof(RakNet::Time) + sizeof(int) + 2; 
}
