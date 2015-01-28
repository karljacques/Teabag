#ifndef keyboardEvent_h__
#define keyboardEvent_h__

struct KeyboardEvent
{
	KeyboardEvent()
	{
		mPressed = 0;
		mReleased = 0;

		mKeycode = 0;
		mKey = 0;
	}

	bool mPressed;
	bool mReleased;

	int mKeycode;
	char mKey;
};
#endif // keyboardEvent_h__
