#include "../base/component.h"

typedef unsigned int EntID;

#ifndef networkComponent_h__
#define networkComponent_h__

class NetworkComponent : public Component
{
public:
	EntID GUID;
};
#endif // networkComponent_h__
