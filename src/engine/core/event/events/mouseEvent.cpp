#include "mouseEvent.h"

MouseEvent::MouseEvent( int eventType ):Event(eventType)
{

}

unsigned int MouseEvent::getSize()
{
	return sizeof(MouseEvent);
}