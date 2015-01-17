#ifndef messageEvent_h__
#define messageEvent_h__

class MessageEvent
{
public:
	MessageEvent()
	{
		message = "NO_MESSAGE";
		from = 0;
	}
	std::string message;
	unsigned int from;
};
#endif // messageEvent_h__
