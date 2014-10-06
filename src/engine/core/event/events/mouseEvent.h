#ifndef mouseEvent_h__
#define mouseEvent_h__

#include "../event.h"

class MouseEvent : public Event
{
public:
	MouseEvent( int eventType );

	unsigned int getSize();

	int32_t mMouseMoveX;
	int32_t mMouseMoveY;
	int m_MouseMoveZ;

	int32_t m_MouseX;
	int32_t m_MouseY;
	int m_MouseZ;

	int m_MouseButton;
	bool m_Released;
	bool m_Pressed;
};
#endif // mouseEvent_h__