#ifndef messageEvent_h__
#define messageEvent_h__

#include "common.h"

class MessageEvent
{
public:
	MessageEvent()
	{
		message = "NO_MESSAGE";
		from = 0;
	}
	std::string message;
	PlayerGUID from;
};
#endif // messageEvent_h__
