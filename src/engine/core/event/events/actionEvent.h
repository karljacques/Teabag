#ifndef actionEvent_h__
#define actionEvent_h__

#include "engine/manager/input/action.h"

struct ActionEvent
{
	ActionEvent()
	{
		eGUID = 0;
		pGUID = 0;
	}

	EntGUID eGUID; // Entity the action concerns
	PlayerGUID pGUID; // Player the action concerns, or is initiated by.

	Action action;
};
#endif // actionEvent_h__
