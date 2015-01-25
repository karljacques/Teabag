#ifndef playerEvent_h__
#define playerEvent_h__

#include "../event.h"

#define USERNAME_LENGTH 20

struct PlayerEvent
{
	PlayerEvent()
	{
		for( int i=0; i < USERNAME_LENGTH; i++ )
		{
			username[i] = 0;
		}
	}

	char username[USERNAME_LENGTH];
	EntID GUID;
};
#endif // playerEvent_h__
