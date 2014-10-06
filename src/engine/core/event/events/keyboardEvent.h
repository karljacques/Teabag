#ifndef keyboardEvent_h__
#define keyboardEvent_h__

#include "../event.h"

class KeyboardEvent : public Event
{
public:
	KeyboardEvent( int eventType );

	unsigned int getSize();

	bool mPressed;
	bool mReleased;

	int mKeycode;
	char mKey;
};
#endif // keyboardEvent_h__
