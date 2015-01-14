#ifndef snapshot_h__
#define snapshot_h__

#define SNAPSHOT_SIZE 25 // 32kb of objects.

#include "common.h"
#include <vector>
#include <RakNetTime.h>

struct Transform
{
	Transform()
	{
		GUID = 0;
		rot = Quat(0,0,0,1);
		angRot = float3(0,0,0);
		pos = float3(0,0,0);
		vel = float3(0,0,0);
	}

	EntID GUID;

	Quat rot;
	
	float3 pos;
	float3 vel;
	float3 angRot;
};

struct Snapshot
{
	RakNet::Time timestamp;
	std::vector<Transform> data;
};
#endif // snapshot_h__
