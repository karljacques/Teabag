#ifndef snapshot_h__
#define snapshot_h__

#define SNAPSHOT_SIZE 25 // 32kb of objects.

#include "common.h"

struct Transform
{
	Transform()
	{
		GUID = 0;
	}

	EntID GUID;

	Quat rot;
	Quat angRot;

	float3 pos;
	float3 vel;
};

struct Snapshot
{
	Snapshot()
	{
		maxindex = 0;
	}
	unsigned int timestamp;
	unsigned int maxindex;

	Transform data[SNAPSHOT_SIZE];
};
#endif // snapshot_h__
