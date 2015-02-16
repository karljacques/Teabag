#ifndef snapshot_h__
#define snapshot_h__

#define SNAPSHOT_SIZE 25 // 32kb of objects.

#include "common.h"

struct Transform
{
	Transform();

	EntGUID GUID;

	Quat rot;
	
	float3 pos;
	float3 vel;
	float3 angRot;
};

struct Snapshot
{
	Snapshot();

	void addTransform( Transform* data );

	RakNet::Time timestamp;
	std::vector<Transform> data;
};
#endif // snapshot_h__
