#include "..\event.h"
#ifndef playerEvent_h__
#define playerEvent_h__

#define USERNAME_LENGTH 20

struct PlayerEvent
{
	PlayerEvent()
	{
	}

	char username[USERNAME_LENGTH];
	EntID GUID;
};
#endif // playerEvent_h__
