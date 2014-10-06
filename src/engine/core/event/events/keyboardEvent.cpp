
#include "keyboardEvent.h"

KeyboardEvent::KeyboardEvent( int eventType ):Event(eventType)
{

}

unsigned int KeyboardEvent::getSize()
{
	return sizeof(KeyboardEvent);
}