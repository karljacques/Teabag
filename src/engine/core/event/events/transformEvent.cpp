#include "transformEvent.h"

TransformEvent::TransformEvent( int eventType ):Event(eventType)
{

}

unsigned int TransformEvent::getSize()
{
	return sizeof(TransformEvent);
}