#ifndef keyboardEvent_h__
#define keyboardEvent_h__

#include "../event.h"

class KeyboardEvent : public Event
{
public:
	KeyboardEvent( int eventType );

	bool mPressed;
	bool mReleased;

	int mKeycode;
};
#endif // keyboardEvent_h__
