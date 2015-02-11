#include "pch.h"
#include "snapshot.h"

Transform::Transform()
{
	GUID = 0;
	rot = Quat(0,0,0,1);
	angRot = float3(0,0,0);
	pos = float3(0,0,0);
	vel = float3(0,0,0);
}

Snapshot::Snapshot()
{
	timestamp = 0;
}

void Snapshot::addTransform(Transform* trans)
{
	// Search the snapshot to see if a Transform for given object already exists.
	int GUID = trans->GUID;

	for( auto i=data.begin(); i!=data.end(); i++ )
	{
		if( i->GUID == GUID )
		{
			// It's already in the snapshot
			// Update this element with the new data
			i->pos = trans->pos;
			i->rot = trans->rot;
			i->vel = trans->vel;
			i->angRot  = trans->angRot;
			
			return;
		}
	}

	// If we've hit this point, it's not in the snapshot and we need to push it back.
	data.push_back(*trans); // Creates a copy. SLOW, could fix.

}