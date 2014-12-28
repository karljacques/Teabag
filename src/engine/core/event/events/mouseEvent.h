#ifndef mouseEvent_h__
#define mouseEvent_h__

struct MouseEvent
{
	MouseEvent()
	{
		mMouseX = 0;
		mMouseY = 0;
		mMouseZ = 0;

		mMouseMoveX = 0;
		mMouseMoveY = 0;
		mMouseMoveZ = 0;

		mMouseButton = 0;
		mReleased = 0;
		mPressed = 0;
	}

	int32_t mMouseMoveX;
	int32_t mMouseMoveY;
	int32_t mMouseMoveZ;

	int32_t mMouseX;
	int32_t mMouseY;
	int32_t mMouseZ;

	int mMouseButton;
	bool mReleased;
	bool mPressed;
};
#endif // mouseEvent_h__