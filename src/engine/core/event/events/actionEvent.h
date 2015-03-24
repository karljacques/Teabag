#ifndef actionEvent_h__
#define actionEvent_h__

struct ActionEvent
{
	ActionEvent()
	{

	}

	EntGUID eGUID;
	PlayerGUID pGUID;

	int action;
};
#endif // actionEvent_h__
