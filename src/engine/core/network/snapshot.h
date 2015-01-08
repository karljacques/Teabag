#ifndef snapshot_h__
#define snapshot_h__

#define SNAPSHOT_SIZE 546 // 32kb of objects.

#include "common.h"

struct Transform
{
	EntID GUID;

	Quat rot;
	Quat angRot;

	float3 pos;
	float3 vel;
};

struct Snapshot
{
	unsigned int timestamp;
	unsigned int maxindex;

	Transform data[SNAPSHOT_SIZE];
};
#endif // snapshot_h__
